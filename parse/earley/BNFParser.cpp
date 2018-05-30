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
 * Forward declarations.
 */
std::string parenRuleSymbol();
BNFProduction parseProduction(TokenQueue& tokens, BNFGrammar& g);


/****************************************************************
 * Symbol parsing.
 ****************************************************************/

/**
 * Parse BNF non-terminal.
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
    } else {
	return nullptr;
    }
}


/**
 * Parse built-in symbol.
 *
 * NOTE To add more built-ins:
 * Add them here and in BNFGrammar::initBuiltIns().
 */
BNFSymbol* parseBuiltIn(TokenQueue& tokens) {
    if (tokens.top().type == TokenType::LESS) {
	tokens.expect(TokenType::LESS);
	std::string type = tokens.expect(TokenType::IDENTIFIER).value;
	tokens.expect(TokenType::GREATER);

	if (type == "epsilon")
	    return new BNFT;
	
	if (type != "id"  &&
	    type != "num" &&
	    type != "str")
	    // Unknown built-in type.
	    throw 1;
	
	return new BNFNT("<" + type + ">");
    } else {
	return nullptr;
    }
}


/**
 * Parse BNF symbol.
 */
BNFSymbol* parseSymbol(TokenQueue& tokens, BNFGrammar& g) {
    BNFSymbol* symbol = nullptr;
    if ((symbol = parseTerminal(tokens))    == nullptr &&
	(symbol = parseNonTerminal(tokens)) == nullptr &&
	(symbol = parseBuiltIn(tokens))     == nullptr)
	// Unknown symbol.
	throw 1; 
    
    return symbol;
}


/****************************************************************
 * Quantifier expansion.
 ****************************************************************/

/**
 * Produce rule matching zero or one quantifier of
 * given symbol in the form:
 * symbol? = symbol | <epsilon>;
 *
 * Returns non terminal refering to produced rule.
 */
BNFNT* zeroOrOne(BNFSymbol* symbol, BNFGrammar& g, bool createsNode=true) {
    std::string ruleSymbol = symbol->toS() + "?";

    BNFRule rule;
    rule.symbol = ruleSymbol;

    BNFProduction zero;
    zero.symbols.push_back(new BNFT);
    if (createsNode)
	zero.nodeLabel = ruleSymbol;

    BNFProduction one;
    one.symbols.push_back(symbol);
    if (createsNode)
	one.nodeLabel = ruleSymbol;
        
    rule.productions.push_back(zero);
    rule.productions.push_back(one);

    g.rules[ruleSymbol] = rule;
    
    return new BNFNT(ruleSymbol);
}


/**
 * Produce rule matching zero or one quantifier of
 * given symbol in the form:
 * symbol* = symbol symbol* | <epsilon>;
 *
 * Returns non terminal refering to produced rule.
 */
BNFNT* zeroOrMore(BNFSymbol* symbol, BNFGrammar& g, bool createsNode=true) {
    std::string ruleSymbol = symbol->toS() + "*";

    BNFRule rule;
    rule.symbol = ruleSymbol;

    BNFProduction zero;
    zero.symbols.push_back(new BNFT);
    if (createsNode)
	zero.nodeLabel = ruleSymbol;
    
    BNFProduction more;
    more.symbols.push_back(symbol);
    more.symbols.push_back(new BNFNT(ruleSymbol));
    if (createsNode)
	more.nodeLabel = ruleSymbol;
        
    rule.productions.push_back(zero);
    rule.productions.push_back(more);

    g.rules[ruleSymbol] = rule;
    
    return new BNFNT(ruleSymbol);
}


/****************************************************************
 * Production parsing.
 ****************************************************************/

/**
 * Parse parentheses.
 */
BNFNT* parseParen(TokenQueue& tokens, BNFGrammar& g) {
    tokens.expect(TokenType::LPAR);
    BNFProduction production = parseProduction(tokens, g);
    tokens.expect(TokenType::RPAR);
    
    std::string ruleSymbol = parenRuleSymbol();

    
    BNFRule rule;
    rule.symbol = ruleSymbol;
    rule.productions.push_back(production);

    g.rules[ruleSymbol] = rule;
    
    return new BNFNT(ruleSymbol);
}


/**
 * Parse BNF production.
 */
BNFProduction parseProduction(TokenQueue& tokens, BNFGrammar& g) {
    BNFProduction production;

    if (tokens.eat(TokenType::COL)) {
	std::string attr = tokens.expect(TokenType::IDENTIFIER).value;
	production.attribute = attr;
    } else {
	production.attribute = "group";
    }
    
    while (tokens.top().type != TokenType::PIPE &&
	   tokens.top().type != TokenType::SEMICOL &&
	   tokens.top().type != TokenType::AT &&
	   tokens.top().type != TokenType::RPAR) {

	bool createsNode;
	BNFSymbol* symbol;
	if (tokens.top().type == TokenType::LPAR) {
	    symbol = parseParen(tokens, g);
	    createsNode = false;
	} else {
	    symbol = parseSymbol(tokens, g);
	    createsNode = true;
	}
	   
	if (tokens.eat(TokenType::QUESTION)) {
	    symbol = zeroOrOne(symbol, g, createsNode);
	} else if (tokens.eat(TokenType::MUL)) {
	    symbol = zeroOrMore(symbol, g, createsNode);
	} else if (tokens.eat(TokenType::PLUS)) {
	    assert(false); // Not implemented.
	}
	
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
 * Returns a new unique symbol for rule expansion of parentheses.
 */
std::string parenRuleSymbol() {
    static int index = 0;

    std::stringstream ss;
    ss << "(" << index++ << ")";
    return ss.str();
}


/**
 * Indexed subrule symbol.
 */
std::string subruleSymbol(std::string symbol, int index) {
    // Ensure no symbol collisions with 'parenRuleSymbol()'.
    assert(!symbol.empty());
    
    std::stringstream ss;
	ss << symbol;
	ss << "(" << index << ")";
    return ss.str();
}


/****************************************************************
 * Rule parsing.
 ****************************************************************/

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
	
	BNFProduction production = parseProduction(tokens, g);
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
