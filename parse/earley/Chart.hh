#pragma once

#include <unordered_set>
#include <vector>
#include <list>

#include "lexer/TokenQueue.hh"

/**
 * Forward declarations.
 */
struct EState;
class BNFProduction;
class BNFSymbol;
class BNFGrammar;;


/**
 * Earley parse chart.
 */
struct EChart {
    /**
     * Rows.
     *
     * Rows are represented both by a linked list and a set to allow iteration
     * over items while iterating and remove duplicate sets respectively.
     * The set contains hash values for all states present in the linked list.
     */
    std::vector<std::list<EState>> s;
    std::vector<std::unordered_set<size_t>> sh;


    /**
     * Constructor.
     */
    EChart(int length);
    
    
    /**
     * Constructor.
     */
    EChart(TokenQueue& tokens) : EChart(tokens.size() + 1) { }

    
    /**
     * Add row.
     * Returns false if the state was already present.
     */
    bool add(BNFGrammar& g, EState state, int k);

    
    /**
     * Get set at index 'k'.
     */
    std::list<EState>& set(int k);

    
    /**
     * Return true if given state is present in row 'k';
     */
    bool contains(EState& state, int k);
};


/**
 * Earley state.
 */
struct EState {
    /**
     * Rule symbol.
     */
    std::string symbol;
    
    /**
     * Production being matched.
     */
    BNFProduction& production;
    /**
     * Origin position.
     */
    int origin;
    
    /**
     * Current position.
     */
    int position;
    
    /**
     * Message saying how state was created.
     */
    std::string message;
    
    /**
     * Previous.
     * Points to the state that this state was scanned or completed from.
     */
    EState* previousState = nullptr;

    
    /**
     * Completed.
     * Points to the state that this state was completed from.
     */
    EState* completedState = nullptr;


    /**
     * Number of children in the resulting AST.
     */
    int childCount;

    
    /**
     * Constructor.
     */
    EState(std::string symbol, BNFProduction& production, int origin)
	: symbol(symbol), production(production), origin(origin), position(0),
	  childCount(0) { }

    
    /**
     * Next symbol.
     */
    BNFSymbol* next() const;


    /**
     * Previous symbol.
     */
    BNFSymbol* previous();


    /**
     * Returns true if state is completely parsed.
     */
    bool complete() const;

    
    /**
     * Hash.
     */
    size_t hash() const;

    
    /**
     * Equality.
     */
    bool operator==(const EState& other) const;

    
    /**
     * To string.
     */
    std::string toS() const;
};


namespace std {
    template<> struct hash<EState> {
	std::size_t operator()(const EState& s) const;
    };
}
