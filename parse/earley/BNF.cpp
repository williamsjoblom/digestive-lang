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
    
    for (BNFSymbol* symbol : symbols) ;
	
}

std::string BNFNT::toS() const {
    return symbol;
}


std::string BNFRule::toS() const {
    std::stringstream ss;
    return ss.str();
}

