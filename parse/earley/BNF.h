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
struct BNFGrammar;


/**
 * Quantifier.
 */
enum class BNFQ {
    ONE,
    ZERO_OR_ONE,
    ZERO_OR_MORE,
    ONE_OR_MORE,
};


/**
 * Symbol.
 */
struct BNFSymbol {
    
    /**
     * Quantifier.
     */
    BNFQ quantifier;

    
    /**
     * Nullable.
     */
    virtual bool nullable(BNFGrammar& g) {
	return
	    quantifier == BNFQ::ZERO_OR_MORE ||
	    quantifier == BNFQ::ZERO_OR_ONE;
    }
    
    
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
     * Identifier.
     */
    std::string identifier;

    
    /**
     * Epsilon constructor.
     */
    BNFT() : type(TokenType::UNKNOWN), value(""), identifier("") { }
    
    
    /**
     * Terminal constructor.
     */
    BNFT(TokenType type, std::string value="", std::string identifier="") :
	type(type), value(value), identifier(identifier)  { }


    /**
     * Return true if this symbol is to be included in the AST.
     */
    bool inTree() { return !identifier.empty(); }

    
    /**
     * Nullable.
     */
    bool nullable(BNFGrammar& g) override;

    
    /**
     * This is a terminal symbol.
     */
    bool terminal() override { return true; }


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
     * Nullable.
     */
    bool nullable(BNFGrammar& g) override;

    
    /**
     * This is a non-terminal symbol.
     */
    bool nonTerminal() override { return true; }

    
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
     * Attribute.
     */
    std::string attribute;

    /**
     * Label of node resulting from production.
     */
    std::string nodeLabel;

    
    /**
     * Does this production result in a AST node?
     */
    bool createsNode() const;

    
    /**
     * Does this production contain only one terminal?
     */
    bool trivial() const;

    
    /**
     * Nullable.
     */
    bool nullable(BNFGrammar& g) const;

    
    /**
     * Hash.
     */
    size_t hash() const;
    
    
    /**
     * To string.
     */
    std::string toS() const;
};


enum class NullableState {
    NON_NULLABLE = 0,
    NULLABLE = 1,
    UNKNOWN = 2
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
     * Nullable state.
     */
    NullableState nullableState = NullableState::UNKNOWN;

    
    /**
     * Productions.
     */
    std::vector<BNFProduction> productions;


    /**
     * Does this rule produce onl
     */
    bool trivial();
    

    /**
     * Nullable.
     */
    bool nullable(BNFGrammar& g);
    
    
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
     * Constructor
     */
    BNFGrammar();

    
    /**
     * Add rule.
     */
    void addRule(BNFRule& rule);

    
    /**
     * Add rule whos production only consists of a single symbol.
     */
    void addTrivialRule(std::string ruleSymbol, BNFSymbol* symbol);

    
    /**
     * To string.
     */
    std::string toS() const;

    
private:
    
    /**
     * Initialize built-in rules.
     */
    void initBuiltIns();
};
