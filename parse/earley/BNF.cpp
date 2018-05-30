#include "BNF.h"

#include <sstream>
#include <assert.h>
#include <unordered_set>

#include "util/Hash.h"

/**
 * Epsilon is represented as an unknown token.
 */
const TokenType EPSILON = TokenType::UNKNOWN;


/****************************************************************
 * Symbol.
 ****************************************************************/

BNFT* BNFSymbol::asTerminal() {
    assert(terminal());
    return dynamic_cast<BNFT*>(this);
}


BNFNT* BNFSymbol::asNonTerminal() {
    assert(nonTerminal());
    return dynamic_cast<BNFNT*>(this);
}


/****************************************************************
 * Terminal.
 ****************************************************************/

bool BNFT::accepts(Token& t) {
    if (value.empty())
	return type == t.type;
    else
	return type == t.type && value == t.value;
}


hash_t BNFT::hash() const {
    return std::hash<int>()(type) ^ std::hash<std::string>()(value);
}


bool BNFT::nullable(BNFGrammar& g) {
    return this->type == EPSILON;
}


std::string BNFT::toS() const {
    std::stringstream ss;
	
    if (!value.empty())
	ss << "\"" << value << "\"";
    else if(type == EPSILON)
	ss << "ϵ";
    else ss << tokenTypeToS[type];
	
    return ss.str();
}


/****************************************************************
 * Non terminal.
 ****************************************************************/

bool BNFNT::nullable(BNFGrammar& g) {
    // Method may cause infinite recursion for self
    // referencing rules in its current state.
    return g.rules[symbol].nullable(g);
}


hash_t BNFNT::hash() const {
    return std::hash<std::string>()(symbol);
}


std::string BNFNT::toS() const {
    return symbol;
}


/****************************************************************
 * Production.
 ****************************************************************/

bool BNFProduction::createsNode() const {
    return !nodeLabel.empty();
}


bool BNFProduction::nullable(BNFGrammar& g) {
    for (BNFSymbol* s : symbols) {
	if (!s->nullable(g))
	    return false;
    }
    
    return true;
}


hash_t BNFProduction::hash() const {
    hash_t h = 0;
    for (BNFSymbol* symbol : symbols)
	h ^= symbol->hash();

    return h;
}


std::string BNFProduction::toS() const {
    std::stringstream ss;
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
	ss << (*it)->toS();
	if (it != symbols.end())
	    ss << ' ';
    }

    return ss.str();
}


/****************************************************************
 * Rule.
 ****************************************************************/

bool BNFRule::nullable(BNFGrammar& g) {
    if (nullableState != NullableState::UNKNOWN)
	return nullableState == NullableState::NULLABLE;
    
    nullableState = NullableState::NULLABLE;

    bool null = false;
    for (BNFProduction& p : productions) {
	if (p.nullable(g)) {
	    null = true;
	    break;
	}
    }

    nullableState =
	null ? NullableState::NULLABLE : NullableState::NON_NULLABLE;
    return null;
}


std::string BNFRule::toS() const {
    std::stringstream ss;

    ss << symbol << " = ";

    for (int i = 0; i < productions.size(); i++) {
	const BNFProduction& p = productions[i];
	if (i > 0) ss << "| ";
	ss << p.toS();
    }
    
    return ss.str();
}


/****************************************************************
 * Grammar
 ****************************************************************/

BNFGrammar::BNFGrammar() {

}


std::string BNFGrammar::toS() const {
    std::stringstream ss;
    for (const auto& rule : rules) {
	ss << rule.second.toS() << std::endl;
    }
    
    return ss.str();
}

