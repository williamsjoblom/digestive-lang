#include "BNF.h"

#include <sstream>
#include <assert.h>

BNFT* BNFSymbol::asTerminal() {
    assert(terminal());
    return dynamic_cast<BNFT*>(this);
}


BNFNT* BNFSymbol::asNonTerminal() {
    assert(nonTerminal());
    return dynamic_cast<BNFNT*>(this);
}


std::string BNFT::toS() const {
    std::stringstream ss;
	
    if (!value.empty()) ss << "\"" << value << "\"";
    else ss << tokenTypeToS[type];
	
    return ss.str();
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
	if (i > 0) ss << std::endl << "| ";
	ss << p.toS();
    }
    
    return ss.str();
}

std::string BNFGrammar::toS() const {
    std::stringstream ss;
    for (const BNFRule& rule : rules) {
	ss << rule.toS() << std::endl;
    }
    
    return ss.str();
}

