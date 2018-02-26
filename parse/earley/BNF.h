#pragma once

#include <string>
#include <vector>
#include <map>

#include "lexer/Token.h"

/**
 * Forward declarations.
 */
struct BNFT;
struct BNFNT;


/**
 * Symbol.
 */
struct BNFSymbol {

    /**
     * Hash.
     */
    virtual size_t hash() const = 0;

    
    /**
     * True if this is a terminal symbol.
     */
    virtual bool terminal() { return false; }

    
    /**
     * True if this is a non terminal symbol.
     */
    virtual bool nonTerminal() { return false; }

    
    /**
     * Cast to terminal symbol.
     */
    BNFT* asTerminal();

    
    /**
     * Cast to non terminal symbol.
     */
    BNFNT* asNonTerminal();

    /**
     * To string.
     */
    virtual std::string toS() const = 0;
};


/**
 * Terminal symbol.
 */
struct BNFT : public BNFSymbol {
    /**
     * Type of terminal token.
     */
    TokenType type;
    
    /**
     * Value of terminal token, can be left empty to match
     * token regardless of value.
     */
    std::string value;

    
    /**
     * Constructor.
     */
    BNFT(TokenType type, std::string value="") : type(type), value(value)  { }


    /**
     * Return true if passed Token is accepted by this symbol.
     */
    bool accepts(Token& t);
    

    /**
     * Hash.
     */
    size_t hash() const override;

    
    /**
     * To string.
     */
    std::string toS() const override;
};


/**
 * Non terminal symbol.
 */
struct BNFNT : public BNFSymbol {
    /**
     * Non terminal symbol.
     */
    std::string symbol;

    
    /**
     * Constructor.
     */
    BNFNT(std::string symbol) : symbol(symbol) { }

    
    /**
     * Hash.
     */
    size_t hash() const override;

    
    /**
     * To string.
     */
    std::string toS() const override;
};


/**
 * Production.
 */
struct BNFProduction {
    /**
     * Symbols.
     */
    std::vector<BNFSymbol*> symbols;

    
    /**
     * Hash.
     */
    size_t hash() const;
    
    
    /**
     * To string.
     */
    std::string toS() const;
};

    
/**
 * Production rule.
 */
struct BNFRule {
    /**
     * Symbol.
     */
    std::string symbol;

    /**
     * Productions.
     */
    std::vector<BNFProduction> productions;

    
    /**
     * To string.
     */
    std::string toS() const;
};


/**
 * Grammar.
 */
struct BNFGrammar {
    /**
     * Rules.
     */
    std::map<std::string, BNFRule> rules;

    
    /**
     * To string.
     */
    std::string toS() const;
};
