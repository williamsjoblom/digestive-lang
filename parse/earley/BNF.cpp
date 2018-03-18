#include "BNF.h"

#include <sstream>
#include <assert.h>

#include "util/Hash.h"


BNFT* BNFSymbol::asTerminal() {
    assert(terminal());
    return dynamic_cast<BNFT*>(this);
}


BNFNT* BNFSymbol::asNonTerminal() {
    assert(nonTerminal());
    return dynamic_cast<BNFNT*>(this);
}


bool BNFT::accepts(Token& t) {
    if (value.empty())
	return type == t.type;
    else
	return type == t.type && value == t.value;
}


std::string BNFT::toS() const {
    std::stringstream ss;
	
    if (!value.empty()) ss << "\"" << value << "\"";
    else ss << tokenTypeToS[type];
	
    return ss.str();
}


bool BNFNT::nullable(BNFGrammar& g) {
    // Method may cause infinite recursion for self
    // referencing rules in its current state.
    return
	BNFSymbol::nullable(g) ||
	g.rules[symbol].nullable(g);
}


hash_t BNFNT::hash() const {
    return std::hash<std::string>()(symbol);
}


hash_t BNFT::hash() const {
    return std::hash<int>()(type) ^ std::hash<std::string>()(value);
}


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


bool BNFT::nullable(BNFGrammar& g) {
    return
	BNFSymbol::nullable(g) ||
	value.empty();
}


std::string BNFNT::toS() const {
    return symbol;
}


bool BNFRule::nullable(BNFGrammar& g) {
    for (BNFProduction& p : productions) {
	if (p.nullable(g))
	    return true;
    }

    return false;
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


std::string BNFGrammar::toS() const {
    std::stringstream ss;
    for (const auto& rule : rules) {
	ss << rule.second.toS() << std::endl;
    }
    
    return ss.str();
}

