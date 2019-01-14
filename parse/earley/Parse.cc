#include "Parse.hh"

#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <catch.hpp>

#include "BNF.hh"
#include "Chart.hh"
#include "ast/Expr.hh"
#include "lexer/TokenQueue.hh"
#include "lexer/Token.hh"
#include "options.hh"
#include "ASTNode.hh"
#include "util/Colors.hh"
#include "BNFParser.hh"
#include "lexer/Lexer.hh"
#include "parse/earley/construct.hh"

using namespace Earley;


/**
 * Forward declarations.
 */
std::string toS(std::list<EState> states);


/**
 * Store debug info in earley states.
 */
bool storeDebugInfo() {
    return dumpParseSets || dumpParseStateTree;
}


/**
 * Prediction.
 *
 * Prediction: For every state in S(k) of the form (X → α • Y β, j)
 * (where j is the origin position as above), add (Y → • γ, k) to S(k)
 * for every production in the grammar with Y on the left-hand side (Y → γ).
 */
inline bool predict(EState& state, BNFGrammar& g, EChart& chart, int k) {
    bool changed = false;

    if (state.complete()) return false;
    if (!state.next()->nonTerminal()) return false;
    
    BNFNT* y = state.next()->asNonTerminal();
    BNFRule& r = g.rules.at(y->symbol);
    
    for (BNFProduction& production : r.productions) {
	EState s(r.symbol, production, k);
	s.previousState = nullptr;
	s.completedState = nullptr;

	if (storeDebugInfo()) {
	    s.message = "predicted from " + state.symbol;
	}
	
	changed |= chart.add(g, s, k);
    }
    
    if (r.nullable(g)) {
	EState s(state);
	s.position++;

	if (storeDebugInfo()) {
	    s.message = "eps skip";
	}
	
	chart.add(g, s, k);
    }

    return changed;
}


/**
 * Scan.
 */
inline bool scan(EState& state, BNFGrammar& g, TokenQueue& tokens,
		 EChart& chart, int k) {
    bool changed = false;
    
    if (state.complete()) return false;
    if (!state.next()->terminal()) return false;
    
    BNFT* a = state.next()->asTerminal();
    Token top = tokens.at(k);
    
    if (a->accepts(top)) {
	EState s(state);
	s.position++;
	s.previousState = &state;
	s.completedState = nullptr;
	
	if (storeDebugInfo()) {
	    s.message = "scanned from '" + top.value + "'";
	}
	
	changed |= chart.add(g, s, k + 1);
    }


    return changed;
}


/**
 * Complete.
 *
 * NOTE In dire need of optimization, both in terms of
 * complexity and caching.
 */
inline bool complete(EState& state, BNFGrammar& g, EChart& chart, int k) {
    bool changed = false;

    if (!state.complete()) return false;
    
    std::string symbol = state.symbol;
    for (EState& s : chart.s[state.origin]) {
	if (!s.complete() && s.next()->nonTerminal() &&
	    s.next()->asNonTerminal()->symbol == symbol) {
	    
	    EState newState(s);
	    newState.position++;
	    newState.previousState = &s;
	    newState.completedState = &state;

	    if (storeDebugInfo()) {
		newState.message = "completed from " + symbol;
	    }
	    
	    changed |= chart.add(g, newState, k);
	}
    }
    
    return changed;
}


/**
 * Process earley state.
 */
inline void processState(BNFGrammar& g, TokenQueue& tokens,
			 EChart& chart, int k) {
    bool changed;

    for (EState& state : chart.s[k]) {
	bool p = predict(state, g, chart, k);
	bool s = scan(state, g, tokens, chart, k);
	bool c = complete(state, g, chart, k);
    }
}


/**
 * To string.
 */
std::string toS(std::list<EState> states) {
    std::stringstream ss;
    for (EState& s : states) {
	ss << s.toS() << std::endl;
    }
    
    return ss.str();
}


/**
 * Dump states in tree form.
 * 'state' is a recognizing state.
 */
void dumpStateTree(EState* state, TokenQueue& tokens,
		   bool verbose=false, std::string indent="") {
    while (state->previousState != nullptr) {
	if (verbose || state->complete())
	    std::cout << indent << state->toS() << std::endl;

	if (state->completedState != nullptr)
	    dumpStateTree(state->completedState, tokens, verbose, indent + "  ");
	
	state = state->previousState;
    }
}


/**
 * Collect states completed states creating nodes in stack 'states'.
 * The 'parent' paramter is a pointer to the last state producing a node
 * and is used for counting children.
 */
int collectStates(std::stack<EState*>& states, EState* s,
		  EState* parent=nullptr) {

    while (s->previousState != nullptr) {
	bool createNode = s->complete() && s->production.createsNode();
	
	if (createNode) {
	    states.push(s);
	    if (parent) parent->childCount++;
	    parent = s;
	}

	if (s->completedState != nullptr) {
	    collectStates(states, s->completedState, parent);
	}
			
	s = s->previousState;
    }
}


/**
 * Build tree from recognizing state.
 */
Node* buildTree(std::stack<EState*> stateStack,
		   const TokenQueue& tokens, ASTNode* parent=nullptr) {

    std::stack<Node*> nodeStack;
    while (!stateStack.empty()) {
	const EState* top = stateStack.top(); stateStack.pop();
	processNode(top->production.nodeLabel, nodeStack,
		    tokens, top);
    }

    if (nodeStack.size() == 1) {
	return nodeStack.top();
    } else {
	std::cout << "Unable to build tree for more than one root node"
		  << std::endl << "Root nodes: " << std::endl;

	assert(false);
    }
}


/**
 * Build tree from recognizing state.
 * TODO remove.
 */
ASTNode* buildIntermediateTree(EState* state, const TokenQueue& tokens,
			       ASTNode* parent=nullptr) {
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
	    ASTNode* child = buildIntermediateTree(state->completedState,
						   tokens, parent);

	    // NOTE Did 'buildIntermediateTree()' create a new node?
	    // I.e. did 'parent' change?
	    if (child != parent)
		parent->addChild(child);
	}
	
	
	state = state->previousState;
    }

    if (state->production.createsNode()) {
	std::cout << "Build AST node: " << parent->label
		  << " symbols:";
	for (std::string s : parent->symbols)
	    std::cout << " " << s;
	std::cout << std::endl;
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
    Node* parse(BNFGrammar& g, std::string topRule, TokenQueue& tokens) {
	initializeConstructTable();
	
	EChart chart(tokens);

	initChart(chart, g, topRule);
	
	for (int k = 0; k <= tokens.size(); k++) {
	    processState(g, tokens, chart, k);
		 
	    if (dumpParseSets) {
		std::cout << "S[" << k << "]:" << " (top: \""
			  << tokens.at(k).toS() << "\")" << std::endl;
		for (EState& state : chart.s[k]) {
		    std::cout << state.toS() << std::endl;
		}
		std::cout << std::endl;
	    }
	}

	 
	Node* tree = nullptr;
	for (EState& state : chart.s[chart.s.size() - 1]) {
	    if (state.origin == 0 &&
		state.symbol == topRule &&
		state.complete()) {
		 
		if (tree != nullptr) {
		    ambiguousParseError();
		}
		 
		if (dumpParseStateTree) {
		    std::cout << "Recognizing state: " << state.toS()
			      << std::endl;
		    std::cout << "State tree:" << std::endl;
		    dumpStateTree(&state, tokens, false);
		}

		std::stack<EState*> stateStack;
		collectStates(stateStack, &state);

		tree = buildTree(stateStack, tokens);
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

	for (EState& state : chart.s[0])
	    predict(state, g, chart, 0);
	
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

	for (EState& state : chart.s[0])
	    predict(state, g, chart, 0);
	
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

    for (EState& state : chart.s[0])
	scan(state, g, tokens, chart, 0);
    s.position = 1;
    REQUIRE(chart.set(1).size() == 1);
    REQUIRE(chart.contains(s, 1));

    for (EState& state : chart.s[1])
	scan(state, g, tokens, chart, 1);
    s.position = 2;
    REQUIRE(chart.set(2).size() == 1);
    REQUIRE(chart.contains(s, 2));
}


TEST_CASE("earley completion") {
    SECTION("trivial completion") {
	std::string grammar =
	    "complete   = \"x\";"
	    "unit       = complete;";
	BNFGrammar g;
	EChart chart = test_createChart(g, grammar, 2);

	// Complete state
	BNFProduction p0;
	p0.symbols.push_back(new BNFT(TokenType::IDENTIFIER, "x"));
	EState completeState("complete", p0, 0);
	completeState.position = 1;

	// Unit state
	BNFProduction p1;
	p1.symbols.push_back(new BNFNT("complete"));
	EState unitState("unit", p1, 0);

	chart.add(g, unitState, 0);
	chart.add(g, completeState, 1);

	for (EState& state : chart.s[1])
	    complete(state, g, chart, 1);
	unitState.position = 1;
	REQUIRE(chart.contains(unitState, 1));
    }
}

