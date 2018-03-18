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
    bool createsNode = tokens.eat(TokenType::DOLLAR);
        
    BNFSymbol* symbol = nullptr;
    if ((symbol = parseTerminal(tokens)) == nullptr &&
	(symbol = parseNonTerminal(tokens)) == nullptr) {
	throw 1;
    }

    if (tokens.eat(TokenType::QUESTION))
	symbol->quantifier = BNFQ::ZERO_OR_ONE;
    else
	symbol->quantifier = BNFQ::ONE;
        
    symbol->createsNode = createsNode;
    return symbol;
}


/**
 * Parse BNF production.
 */
BNFProduction parseProduction(TokenQueue& tokens) {
    BNFProduction production;

    if (tokens.eat(TokenType::COL)) {
	std::string attr = tokens.expect(TokenType::IDENTIFIER).value;
	production.attribute = attr;
    } else {
	production.attribute = "group";
    }
    
    while (tokens.top().type != TokenType::PIPE &&
	   tokens.top().type != TokenType::SEMICOL &&
	   tokens.top().type != TokenType::AT) {
	BNFSymbol* symbol = parseSymbol(tokens);
	production.symbols.push_back(symbol);
    }

    std::string nodeLabel = "";
    if (tokens.eat(TokenType::AT)) {
	nodeLabel = tokens.expect(TokenType::IDENTIFIER).value;
    }

    production.nodeLabel = nodeLabel;

    return production;
}


/**
 * Indexed subrule symbol.
 */
std::string subruleSymbol(std::string symbol, int index) {
    std::stringstream ss;
    ss << symbol;
    ss << "(" << index << ")";
    return ss.str();
}


/**
 * Make production left associative.
 */
void makeLeftAssociative(BNFProduction& production, std::string symbol, int subruleIndex) {
    bool first = true;
    for (int i = 0; i < production.symbols.size(); i++) {
	BNFSymbol* s = production.symbols[i];
	if (s->nonTerminal()) {
	    BNFNT* nt = s->asNonTerminal();
	    if (nt->symbol != symbol) continue;
	    
	    if (first) {
		nt->symbol = subruleSymbol(symbol, subruleIndex);
		first = false;
	    } else {
		nt->symbol = subruleSymbol(symbol, subruleIndex - 1);
	    }
	}
    }
}


/**
 * Make production right associative.
 */
void makeRightAssociative(BNFProduction& production, std::string symbol, int subruleIndex) {
    bool first = true;
    for (int i = production.symbols.size() - 1; i >= 0; i--) {
	BNFSymbol* s = production.symbols[i];
	if (s->nonTerminal()) {
	    BNFNT* nt = s->asNonTerminal();
	    if (nt->symbol != symbol) continue;
	    
	    if (first) {
		nt->symbol = subruleSymbol(symbol, subruleIndex);
		first = false;
	    } else {
		nt->symbol = subruleSymbol(symbol, subruleIndex - 1);
	    }
	}
    }
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
	    
	    rule.symbol = subruleSymbol(symbol, subrules.size());
	    
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
	
	previous = &top;
    }


    for (int i = 0; i < subrules.size(); i++) {
	BNFRule& subrule = subrules[i];
	for (int j = 0; j < subrule.productions.size(); j++) {
	    BNFProduction& production = subrule.productions[j];

	    if (production.attribute == "left")
		makeLeftAssociative(production, symbol, i);
	    else if (production.attribute == "right")
		makeRightAssociative(production, symbol, i);
	    else if (production.attribute == "group")
		continue;
	}
    }
    
    
    for (int i = 0; i < subrules.size() - 1; i++) {
	BNFRule& subrule = subrules[i];
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
