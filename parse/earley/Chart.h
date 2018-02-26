#pragma once

#include <unordered_set>
#include <vector>

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
     */
    std::vector<std::unordered_set<EState>> s;

    
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
     * Constructor.
     */
    EState(BNFProduction& production, int origin)
	: production(production), origin(origin), position(0) { }

    
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
