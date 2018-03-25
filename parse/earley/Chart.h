#pragma once

#include <unordered_set>
#include <vector>
#include <list>

/**
 * Forward declarations.
 */
struct EState;
class BNFProduction;
class BNFSymbol;
class BNFGrammar;
class TokenQueue;


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
    EChart(TokenQueue& tokens);

    
    /**
     * Add row.
     */
    void add(EState row, int k);
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
     * Previous.
     * Points to the state that this state was scanned or completed from.
     */
    const EState* previousState = nullptr;

    
    /**
     * Completed.
     * Points to the state that this state was completed from.
     */
    const EState* completedState = nullptr;

    
    /**
     * Constructor.
     */
    EState(std::string symbol, BNFProduction& production, int origin)
	: symbol(symbol), production(production), origin(origin), position(0) { }

    
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
