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
#include "options.h"
#include "ASTNode.h"

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
	
	if (y->nullable(g)) {
	    // for (auto it = chart.s[k].begin(); it != chart.s[k].end(); ++it) {
	    // 	EState& now = *it;
	    // 	if (now == state) break;
	    // 	if (now.complete()) continue;
	    // 	if (now.origin != k) continue;
	    // 	if (now.symbol != y->symbol) continue;
		
	    // 	EState s(state);
	    // 	s.position++;
	    // 	s.previousState = &state;
	    // 	s.completedState = &now;
		
	    // 	changed |= chart.add(g, s, k);
	    // }
	    
	    EState s(state);
	    s.position++;

	    s.message = "predicted from ϵ";
	    
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


const EState* nextNonIntermediate(const EState* state) {
    if (state->complete() &&
	state->production.createsNode())
	return state;
    else if (state->completedState != nullptr)
	return nextNonIntermediate(state->completedState);
    else
	return state;
}


ASTNode* buildIntermediateTree(const EState* state, ASTNode* parent=nullptr) {
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
	    ASTNode* child = buildIntermediateTree(s->completedState, parent);
	    
	    // Did buildIntermediateTree() create a new node?
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
    ASTNode* parse(BNFGrammar& g, std::string rule, TokenQueue& tokens) {
	EChart chart(tokens);
	
	 // S(0)
	 BNFRule& rootRule = g.rules[rule];
	 for (BNFProduction& p : rootRule.productions) {
	     EState initial(rootRule.symbol, p, 0);
	     chart.add(g, initial, 0);
	 }
	
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


	 for (const EState& state : chart.s[chart.s.size() - 1]) 
	    if (state.origin == 0 &&
		state.symbol == rule) {
		if (verbose) {
		    std::cout << "Recognizing state: " << state.toS() << std::endl;
		    std::cout << "State tree:" << std::endl;
		    dumpStateTree(&state, tokens, false);
		}

		ASTNode* tree = buildIntermediateTree(&state);
		return tree;
	    }
	
	return nullptr;
    }
}
