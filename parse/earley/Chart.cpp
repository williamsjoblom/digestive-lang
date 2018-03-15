#include "Chart.h"

#include <sstream>
#include <functional>

#include "BNF.h"
#include "lexer/TokenQueue.h"
#include "util/Hash.h"


EChart::EChart(TokenQueue& tokens) {
    int stateSize = tokens.size() + 1;
    for (int i = 0; i < stateSize; i++) {
	s.push_back(std::list<EState>());
	sh.push_back(std::unordered_set<size_t>());
    }
	
}


void EChart::add(EState state, int k) {
    hash_t hash = state.hash();
    if (sh[k].find(hash) == sh[k].end()) {
	sh[k].insert(hash);
	s[k].insert(s[k].end(), state);
    } else {
	std::cout << "Duplicate state in 5: " << state.toS() << std::endl;
    }
}

BNFSymbol* EState::next() const {
    if (position >= 0 &&
	position < production.symbols.size())
	return production.symbols[position];
    else
	return nullptr;
}


BNFSymbol* EState::previous() {
    if (position - 1 >= 0 &&
	position - 1 < production.symbols.size())
	return production.symbols[position - 1];
    else
	return nullptr;
}


bool EState::complete() const {
    return production.symbols.size() == position;
}


hash_t EState::hash() const {
    // TODO: use a better hash function for integers since std::hash for
    // integers is the identity function. Doing an XOR for the 2 integers
    // in this context WILL result in collisions.
    //
    // Negating 'position' works as of now. If the parser starts behaving
    // funky again: hash the integers in a more uniform manner.
    return
	std::hash<std::string>()(symbol) ^ production.hash() ^
	orderedHash<2>(origin, position);
}


bool EState::operator==(const EState& other) const {
    return hash() == other.hash();
}


std::string EState::toS() const {
    std::stringstream ss;

    ss << "(" << symbol << " → ";
    
    for (int i = 0; i < production.symbols.size(); i++) {
	if (i == position) ss << "• ";
	ss << production.symbols[i]->toS() << " ";
    }

    if (position == production.symbols.size())
	ss << "•";

    ss << ", " << origin << ")";
    
    return ss.str();
}


namespace std {
    std::size_t hash<EState>::operator()(const EState& s) const {
	return s.hash();
    }
}

