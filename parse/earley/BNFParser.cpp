#include "BNFParser.h"

#include <string>

#include "BNF.h"
#include "lexer/Lexer.h"
#include "lexer/TokenQueue.h"
#include "lexer/Token.h"

/**
 * Parse BNF terminal.
 */
BNFNT* parseNonTerminal(TokenQueue& tokens) {
    if (tokens.top().type == TokenType::IDENTIFIER) {
	std::string value = tokens.pop().value;
	return new BNFNT(value);
    } else {
	return nullptr;
    }
}

/**
 * Parse BNF terminal.
 */
BNFT* parseTerminal(TokenQueue& tokens) {
    if (tokens.top().type == TokenType::STRING) {
	std::string str = tokens.pop().value;
	std::string content = str.substr(1, str.length() - 2); // Strip quotes.
	
	Lexer l(content);
	Token t = l.read();
	return new BNFT(t.type, t.value);
    } else if (tokens.top().type == TokenType::LESS) {
	tokens.expect(TokenType::LESS);

	std::string type = tokens.expect(TokenType::IDENTIFIER).value;
	TokenType t;
	if (type == "id") {
	    t = TokenType::IDENTIFIER;
	} else if (type == "num") {
	    t = TokenType::NUMBER;
	} else if (type == "str") {
	    t = TokenType::STRING;
	} else {
	    throw 1;
	}
	
	tokens.expect(TokenType::GREATER);

	return new BNFT(t);
    } else {
	return nullptr;
    }
}


/**
 * Parse BNF symbol.
 */
BNFSymbol* parseSymbol(TokenQueue& tokens) {
    BNFSymbol* symbol;
    if ((symbol = parseTerminal(tokens)) != nullptr) return symbol;
    if ((symbol = parseNonTerminal(tokens)) != nullptr) return symbol;

    throw 1;
}


/**
 * Parse BNF production.
 */
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
    
    tokens.expect(TokenType::ASSIGN);
    
    do {
	BNFProduction production = parseProduction(tokens);
	rule.productions.push_back(production);
    } while (tokens.eat(TokenType::PIPE));
    
    tokens.expect(TokenType::SEMICOL);
    
    return rule;
}


namespace Earley {
    BNFGrammar parseGrammar(TokenQueue& tokens) {
	BNFGrammar g;
	while (!tokens.empty()) {
	    BNFRule rule = parseRule(tokens);
	    g.rules.push_back(rule);
	}

	return g;
    }
}
