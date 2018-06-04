#include "Parse.h"

#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <catch.hpp>

#include "BNF.h"
#include "Chart.h"
#include "ast/Expr.h"
#include "lexer/TokenQueue.h"
#include "lexer/Token.h"
#include "options.h"
#include "ASTNode.h"
#include "util/Colors.h"
#include "BNFParser.h"
#include "lexer/Lexer.h"

using namespace Earley;


/**
 * Forward declarations.
 */
std::string toS(std::list<EState> states);


/**
 * Prediction.
 *
 * Prediction: For every state in S(k) of the form (X → α • Y β, j)
 * (where j is the origin position as above), add (Y → • γ, k) to S(k)
 * for every production in the grammar with Y on the left-hand side (Y → γ).
 */
bool predict(BNFGrammar& g, EChart& chart, int k) {
    bool changed = false;

    for (EState& state : chart.s[k]) {
	if (state.complete()) continue;
	if (!state.next()->nonTerminal()) continue;
	
	BNFNT* y = state.next()->asNonTerminal();
	BNFRule& r = g.rules.at(y->symbol);
	
	for (BNFProduction& production : r.productions) {
	    EState s(r.symbol, production, k);
	    s.previousState = nullptr;
	    s.completedState = nullptr;

	    s.message = "predicted from " + state.symbol;
	    
	    changed |= chart.add(g, s, k);
	}
    }
    
    return changed;
}


/**
 * Scan.
 */
bool scan(BNFGrammar& g, TokenQueue& tokens, EChart& chart, int k) {
    bool changed = false;
    
    for (const EState& state : chart.s[k]) {
	if (state.complete()) continue;
	if (!state.next()->terminal()) continue;
	
	BNFT* a = state.next()->asTerminal();
	Token top = tokens.at(k);
	
	if (a->accepts(top)) {
	    EState s(state);
	    s.position++;
	    s.previousState = &state;
	    s.completedState = nullptr;
	    
	    s.message = "scanned from '" + top.value + "'";
	    
	    changed |= chart.add(g, s, k + 1);
	}
    }

    return changed;
}


/**
 * Complete.
 *
 * TODO Nasty nesting depth, please make me pretty!
 * NOTE In dire need of optimization, both in terms of
 * complexity and caching.
 */
bool complete(BNFGrammar& g, EChart& chart, int k) {
    bool changed = false;

    for (const EState& state : chart.s[k]) {
	if (!state.complete()) continue;
	
	for (const EState& completeState : chart.s[k]) {
	    if (!completeState.complete()) continue;
	
	    std::string symbol = completeState.symbol;
	    for (const EState& s : chart.s[completeState.origin]) {
		if (!s.complete() && s.next()->nonTerminal() &&
		    s.next()->asNonTerminal()->symbol == symbol) {
		
		    EState newState(s);
		    newState.position++;
		    newState.previousState = &s;
		    newState.completedState = &completeState;

		    newState.message = "completed from " + symbol;
				
		    changed |= chart.add(g, newState, k);

		    // Completed from a nullable?
		    if (state.production.nullable(g)) {
			EState ns(s);
			ns.previousState = &s;
			ns.completedState = &completeState;
			ns.message += "/ϵ-forward";
			changed |= chart.add(g, ns, k);

			if (false && verbose) {
			    std::cout << "ϵ-forward:" << std::endl
				      << "       state = " << state.toS() << std::endl
				      << "    complete = " << completeState.toS() << std::endl
				      << "           s = " << s.toS() << std::endl
				      << "    newState = " << newState.toS() << std::endl
				      << "          ns = " << ns.toS() << std::endl << std::endl;
			}
		    }
		}
	    }
	}
    }
    
    return changed;
}


/**
 * Process earley state.
 */
void processState(BNFGrammar& g, TokenQueue& tokens, EChart& chart, int k) {
    bool changed;

    do {
	changed = false;

	bool p = predict(g, chart, k);
	bool s = scan(g, tokens, chart, k);
        bool c = complete(g, chart, k);
	
	changed = p | s | c;
    } while (changed);
}


/**
 * To string.
 */
std::string toS(std::list<EState> states) {
    std::stringstream ss;
    for (const EState& s : states) {
	ss << s.toS() << std::endl;
    }
    
    return ss.str();
}


void dumpStateTree(const EState* state, TokenQueue& tokens, bool verbose=false, std::string indent="") {
    while (state->previousState != nullptr) {
	if (verbose || state->complete())
	    std::cout << indent << state->toS() << std::endl;

	if (state->completedState != nullptr)
	    dumpStateTree(state->completedState, tokens, verbose, indent + "  ");
	
	state = state->previousState;
    }
}


/**
 * Build tree from recognizing state.
 */
ASTNode* buildIntermediateTree(const EState* state, const TokenQueue& tokens, ASTNode* parent=nullptr) {
    while (state->previousState != nullptr) {
	if (state->complete()) {
	    if (state->production.createsNode())
		parent = new ASTNode(state->production.nodeLabel);

	    if (state->production.trivial()) {
		BNFSymbol* symbol = state->production.symbols[0];
		BNFT* terminal = symbol->asTerminal();

		if (terminal->inTree()) {
		    int index = state->origin;
		    std::string value = tokens.at(index).value;
		    parent->addSymbol(value);
		}
	    }
	}

	if (state->completedState != nullptr) {
	    ASTNode* child = buildIntermediateTree(state->completedState, tokens, parent);

	    // NOTE Did 'buildIntermediateTree()' create a new node?
	    // I.e. did 'parent' change?
	    if (child != parent)
		parent->addChild(child);
	}
	
	
	state = state->previousState;
    }
    
    return parent;
}


/**
 * Initialize chart.
 * Seed S(0) with the top-level rule.
 */
void initChart(EChart& chart, BNFGrammar& g, std::string topRule) {
    BNFRule& rootRule = g.rules[topRule];
    for (BNFProduction& p : rootRule.productions) {
	EState initial(rootRule.symbol, p, 0);
	chart.add(g, initial, 0);
    }
}


/**
 * Ambiguous parse error.
 */
void ambiguousParseError() {
    std::cout << KRED
	      << "Grammar ambiguoity resulted in multiple parse trees!"
	      << RST;
    throw 1;
}


namespace Earley {
    ASTNode* parse(BNFGrammar& g, std::string topRule, TokenQueue& tokens) {
	EChart chart(tokens);

	initChart(chart, g, topRule);
	
	for (int k = 0; k <= tokens.size(); k++) {
	    processState(g, tokens, chart, k);
		 
	    if (verbose) {
		std::cout << "S[" << k << "]:" << " (top: \"" << tokens.at(k).toS() << "\")" << std::endl;
		for (const EState& state : chart.s[k]) {
		    std::cout << state.toS() << std::endl;
		}
		std::cout << std::endl;
	    }
	}

	 
	ASTNode* tree = nullptr;
	for (const EState& state : chart.s[chart.s.size() - 1]) {
	    if (state.origin == 0 &&
		state.symbol == topRule &&
		state.complete()) {
		 
		if (tree != nullptr) {
		    ambiguousParseError();
		}
		 
		if (verbose) {
		    std::cout << "Recognizing state: " << state.toS() << std::endl;
		    std::cout << "State tree:" << std::endl;
		    dumpStateTree(&state, tokens, false);
		}

		tree = buildIntermediateTree(&state, tokens);
	    }
	}

	return tree;
    }
}


/****************************************************************
 * Unit tests.
 ****************************************************************/

/**
 * Helper function creating a chart (and grammar) from grammar string.
 */
EChart test_createChart(BNFGrammar& g, std::string grammar, int chartLength) {
    Lexer l(grammar);
    TokenQueue tokens = l.readAll();
    g = parseGrammar(tokens);
    
    EChart chart(chartLength);
    initChart(chart, g, "unit");
    return chart;
}


TEST_CASE("earley prediction") {
    SECTION("trivial prediction") {
	std::string grammar =
	    "Predicted = \"0\";"
	    "unit      = Predicted;";
	BNFGrammar g;
	EChart chart = test_createChart(g, grammar, 1);

	predict(g, chart, 0);
	
	REQUIRE(chart.s[0].size() == 2);

	BNFProduction p;
	p.symbols.push_back(new BNFT(TokenType::NUMBER, "0"));

	EState s("Predicted", p, 0);
	REQUIRE(chart.contains(s, 0));
    }

    SECTION("chained prediction") {
	std::string grammar =
	    "Predicted1 = \"0\";"
	    "Predicted0 = Predicted1;"
	    "unit       = Predicted0;";
	BNFGrammar g;
	EChart chart = test_createChart(g, grammar, 1);

	predict(g, chart, 0);
	
	REQUIRE(chart.s[0].size() == 3);

	//Predicted0
	BNFProduction p0;
	p0.symbols.push_back(new BNFNT("Predicted1"));

	EState s0("Predicted0", p0, 0);
	REQUIRE(chart.contains(s0, 0));
	
	// Predicted1
	BNFProduction p1;
	p1.symbols.push_back(new BNFT(TokenType::NUMBER, "0"));

	EState s1("Predicted1", p1, 0);
	REQUIRE(chart.contains(s1, 0));
    }
}


TEST_CASE("earley scanning") {
    std::string grammar =
	"unit       = \"x\" \"y\";";
    BNFGrammar g;
    EChart chart = test_createChart(g, grammar, 3);

    TokenQueue tokens = Lexer("x y").readAll();

    BNFProduction p;
    p.symbols.push_back(new BNFT(TokenType::IDENTIFIER, "x"));
    p.symbols.push_back(new BNFT(TokenType::IDENTIFIER, "y"));
    
    EState s("unit", p, 0);

    scan(g, tokens, chart, 0);
    s.position = 1;
    REQUIRE(chart.set(1).size() == 1);
    REQUIRE(chart.contains(s, 1));

    scan(g, tokens, chart, 1);
    s.position = 2;
    REQUIRE(chart.set(2).size() == 1);
    REQUIRE(chart.contains(s, 2));
}
