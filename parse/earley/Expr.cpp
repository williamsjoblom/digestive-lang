#include "Expr.h"

#include <string>
#include <sstream>
#include <vector>
#include <stack>

#include "BNF.h"
#include "Chart.h"
#include "ast/Expr.h"
#include "lexer/TokenQueue.h"
#include "lexer/Token.h"
#include "globals.h"
#include "ASTNode.h"

using namespace Earley;


/**
 * Prediction.
 *
 * Prediction: For every state in S(k) of the form (X → α • Y β, j)
 * (where j is the origin position as above), add (Y → • γ, k) to S(k)
 * for every production in the grammar with Y on the left-hand side (Y → γ).
 */
void predict(BNFGrammar& g, const EState& state, EChart& chart, int k) {
    assert(state.next()->nonTerminal());

    BNFNT* y = state.next()->asNonTerminal();
    BNFRule& r = g.rules.at(y->symbol);

    for (BNFProduction& production : r.productions) {
	EState s(r.symbol, production, k);
	s.previousState = nullptr;
	s.completedState = nullptr;
	
	chart.add(s, k);
    }

    // To allow for epsilon symbols:
    // if (state.next()->nullable(g)) {
    // 	EState s(state);
    // 	s.position++;
    // 	s.previousState = nullptr;
    // 	s.completedState = nullptr;
	    
    // 	chart.add(s, k);
    // }
}


/**
 * Scan.
 */
void scan(BNFGrammar& g, TokenQueue& tokens, const EState& state, EChart& chart, int k) {
    BNFT* a = state.next()->asTerminal();
    Token top = tokens.at(k);
    
    if (a->accepts(top)) {
	EState s(state);
	s.position++;
	s.previousState = &state;
	s.completedState = nullptr;
	
	chart.add(s, k + 1);
    }

    // To allow for epsilon symbols:
    // if (state.next()->nullable(g)) {
    // 	EState s(state);
    // 	s.position++;
	
    // 	chart.add(s, k);
    // }
}


/**
 * Complete.
 */
void complete(TokenQueue& tokens, const EState& state, EChart& chart, int k) {
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
				
		chart.add(newState, k);
	    }
	}
    }
}


/**
 * Process earley state.
 */
void processState(BNFGrammar& g, TokenQueue& tokens, const EState& state, EChart& chart, int k) {
    if (!state.complete()) {
	if (state.next()->nonTerminal()) {
	    predict(g, state, chart, k);
	} else {
	    scan(g, tokens, state, chart, k);
	}
    } else {
	complete(tokens, state, chart, k);
    }

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


void dumpAST(const EState* state, TokenQueue& tokens, std::string indent="") {
    while (state->previousState != nullptr) {
	if (state->complete())
	    std::cout << indent << state->toS() << std::endl;

	if (state->completedState != nullptr)
	    dumpAST(state->completedState, tokens, indent + "  ");
	
	state = state->previousState;
    }
}


const EState* nextNonIntermediate(const EState* state) {
    if (state->complete() &&
	state->production.createsNode())
	return state;
    else if (state->completedState != nullptr)
	return nextNonIntermediate(state->completedState);
    else
	return state;
}


ASTNode* buildAST(const EState* state, ASTNode* parent=nullptr) {
    state = nextNonIntermediate(state);
    
    if (state->production.createsNode())
	parent = new ASTNode(state->production.nodeLabel);
    else if (parent == nullptr)
	// TODO replace with error message.
	// Happens when the 'unit' rule does not create a node.
	assert(false);

    const EState* s = state;
    while (s->previousState != nullptr) {
	if (s->completedState != nullptr) {
	    ASTNode* child = buildAST(s->completedState, parent);
	    
	    // Did buildAST() create a new node?
	    // TODO returning the parent if no node was created
	    // is not very pretty. We can do better!!!
	    if (child != parent)
		parent->addChild(child);
	}
	
	s = s->previousState;
    }
    
    return parent;
}


namespace Earley {
    bool parse(BNFGrammar& g, std::string rule, TokenQueue& tokens) {
	EChart chart(tokens);
	
	// S(0)
	BNFRule& rootRule = g.rules[rule];
	for (BNFProduction& p : rootRule.productions) {
	    EState initial(rootRule.symbol, p, 0);
	    chart.add(initial, 0);
	}
	

	for (int k = 0; k <= tokens.size(); k++) {
	    int sz = 0;
	    for (const EState& state : chart.s[k]) {
		processState(g, tokens, state, chart, k);
		sz++;
	    }

	    if (verbose) {
		std::cout << "S[" << k << "]:" << " (top: \"" << tokens.at(k).toS() << "\")" << std::endl;
		for (const EState& state : chart.s[k]) {
		    std::cout << state.toS() << std::endl;
		}
		std::cout << std::endl;
	    }
	}


	for (const EState& state : chart.s[chart.s.size() - 1]) 
	    if (state.origin == 0 &&
		state.symbol == rule) {
		std::cout << "Recognizing state: " << state.toS() << std::endl;
		std::cout << "dumpAST():" << std::endl;
		dumpAST(&state, tokens);

		ASTNode* ast = buildAST(&state);
		std::cout << std::endl << ast->toS() << std::endl;
		    
		return true;
	    }
	
	return false;
    }
}
