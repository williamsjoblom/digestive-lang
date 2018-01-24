#include "BNFParser.h"

#include <string>

#include "BNF.h"
#include "lexler/Lexer.h"
#include "lexer/TokenQueue.h"
#include "lexer/Token.h"

BNFT* parseTerminal(TokenQueue& tokens) {
    if (tokens.top().type == TokenType::STRING) {
	Lexer l;
	Token t = tokens.pop();
	return new BNFT(t.);
    }
}


BNFSymbol* parseSymbol(TokenQueue& tokens) {
    BNFT* terminal = parseTerminal(tokens);
    if (terminal != nullptr) return terminal;
}


BNFProduction parseProduction(TokenQueue& tokens) {
    BNFProduction production;
    
    while (tokens.top().type != TokenType::PIPE &&
	   tokens.top().type != TokenType::SEMICOL) {
	BNFSymbol* symbol = parseSymbol(tokens);
	production.symbols.push_back(symbol);
    }

    return production;
}

/**
 * Parse BNF rule.
 */
BNFRule parseRule(TokenQueue& tokens) {
    BNFRule rule;
    rule.symbol = tokens.expect(TokenType::IDENTIFIER).value;
    
    tokens.expect(TokenType::EQ);
    
    do {
	BNFProduction production = parseProduction(tokens);
	rule.productions.push_back(production);
    } while (tokens.eat(TokenType::PIPE));
    
    tokens.expect(TokenType::SEMICOL);
    
    return rule;
}


namespace Earley {
    BNFGrammar parseGrammar(TokenQueue& tokens) {
	
    }
}
