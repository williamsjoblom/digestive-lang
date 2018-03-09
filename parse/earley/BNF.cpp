#include "BNF.h"

#include <sstream>
#include <assert.h>
#include <functional>


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


size_t BNFNT::hash() const {
    return std::hash<std::string>()(symbol);
}


size_t BNFT::hash() const {
    return std::hash<int>()(type) ^ std::hash<std::string>()(value);
}


size_t BNFProduction::hash() const {
    size_t h = 0;
    for (BNFSymbol* symbol : symbols)
	h ^= symbol->hash();

    return h;
}



std::string BNFProduction::toS() const {
    std::stringstream ss;
    for (BNFSymbol* symbol : symbols)
	ss << symbol->toS() << " ";
    
    return ss.str();
}


std::string BNFNT::toS() const {
    return symbol;
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

