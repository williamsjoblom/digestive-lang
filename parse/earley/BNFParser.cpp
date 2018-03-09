#include "BNFParser.h"

#include <string>
#include <stack>
#include <sstream>
#include <assert.h>
#include <cstdlib>

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

    tokens.top().dump();
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
BNFRule parseRule(TokenQueue& tokens, BNFGrammar& g) {
    std::vector<BNFRule> subrules;
    std::string symbol = tokens.expect(TokenType::IDENTIFIER).value;
    
    BNFRule rule;
    tokens.expect(TokenType::ASSIGN);

    do {
	if (!rule.productions.empty() &&
	    tokens.eat(TokenType::PIPE)) {
	    
	    std::stringstream ss;
	    ss << symbol << "(" << subrules.size() << ")";
	    rule.symbol = ss.str();
	    
	    subrules.push_back(rule);
	    rule = BNFRule();
	}
	
	BNFProduction production = parseProduction(tokens);
	rule.productions.push_back(production);
    } while (tokens.eat(TokenType::PIPE));

    subrules.push_back(rule);
    
    tokens.expect(TokenType::SEMICOL);

    BNFRule mainRule = subrules.back();
    mainRule.symbol = symbol;
    
    BNFRule* previous = &mainRule;
    for (int i = subrules.size() - 2; i >= 0; i--) {
	BNFRule& top = subrules[i];
	
	BNFProduction subruleProduction;
	subruleProduction.symbols.push_back(new BNFNT(top.symbol));
	previous->productions.push_back(subruleProduction);
	
	std::cout << "Add subrule" << std::endl;
	
	previous = &top;
    }


    for (int i = 0; i < subrules.size() - 1; i++) {
	BNFRule& subrule = subrules[i];
	for (int i = 1; i < subrule.productions.size(); i++) {
	    
	}
    }
    
    
    for (int i = 0; i < subrules.size() - 1; i++) {
	BNFRule& subrule = subrules[i];

	std::cout << subrule.toS() << std::endl;
	
	g.rules[subrule.symbol] = subrule;
    }

    return mainRule;
}


namespace Earley {
    BNFGrammar parseGrammar(TokenQueue& tokens) {
	BNFGrammar g;
	while (!tokens.empty()) {
	    BNFRule rule = parseRule(tokens, g);
	    g.rules[rule.symbol] = rule;
	}

	std::cout << g.toS() << std::endl;

	return g;
    }
}
