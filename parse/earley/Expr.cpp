#include "Expr.h"

#include <string>
#include <sstream>
#include <vector>

#include "BNF.h"
#include "Chart.h"
#include "ast/Expr.h"
#include "lexer/TokenQueue.h"
#include "lexer/Token.h"

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
	EState s(production, k);
	chart.add(s, k);
    }
}


/**
 * Scan.
 */
void scan(TokenQueue& tokens, const EState& state, EChart& chart, int k) {
    BNFT* a = state.next()->asTerminal();
    Token top = tokens.at(k);

    if (a->accepts(top)) {
	EState s(state);
	s.position++;
	chart.add(s, k + 1);
    }
}


/**
 * Complete.
 */
void complete(TokenQueue& tokens, const EState& state, EChart& chart, int k) {
    for (const EState& s : chart.s[state.origin]) {
	if (state.next()->nonTerminal()) {
	    EState newState(s);
	    chart.add(newState, k);
	}
    }
}


/**
 * To string.
 */
std::string toS(std::unordered_set<EState> states) {
    std::stringstream ss;
    for (const EState& s : states) {
	ss << s.toS() << std::endl;
    }
    
    return ss.str();
}


namespace Earley {
    bool parse(BNFGrammar& g, std::string rule, TokenQueue& tokens) {
	EChart chart(tokens);
	
	// S(0)
	BNFRule& rootRule = g.rules[rule];
	for (BNFProduction& p : rootRule.productions) {
	    EState initial(p, 0);
	    chart.add(initial, 0);
	}
	

	for (int k = 0; k <= tokens.size(); k++) {
	    for (const EState& state : chart.s[k]) {
		if (!state.complete()) {
		    if (state.next()->nonTerminal()) {
			predict(g, state, chart, k);
		    } else {
			scan(tokens, state, chart, k);
		    }
		} else {
		    complete(tokens, state, chart, k);
		}
	    }
	}

	std::cout << "S[" << chart.s.size() << "]:"
		  << std::endl << toS(chart.s[chart.s.size() - 1]);
	
	return false;
    }
}
