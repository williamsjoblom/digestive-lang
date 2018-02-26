#include "Chart.h"

#include <sstream>
#include <functional>

#include "BNF.h"
#include "lexer/TokenQueue.h"


EChart::EChart(TokenQueue& tokens) {
    int stateSize = tokens.size() + 1;
    for (int i = 0; i < stateSize; i++) {
	s.push_back(std::unordered_set<EState>());
    }
	
}


void EChart::add(EState state, int k) {
    s[k].insert(state);
}


BNFSymbol* EState::next() const {
    if (position > 0 &&
	position < production.symbols.size())
	return production.symbols[position];
    else
	return nullptr;
}


BNFSymbol* EState::previous() {
    if (position - 1 > 0 &&
	position - 1 < production.symbols.size())
	return production.symbols[position - 1];
    else
	return nullptr;
}


bool EState::complete() const {
    return production.symbols.size() == position;
}


size_t EState::hash() const {
    return production.hash() ^ std::hash<int>()(origin) ^ std::hash<int>()(position);
}


bool EState::operator==(const EState& other) const {
    return hash() == other.hash();
}


std::string EState::toS() const {
    std::stringstream ss;

    for (int i = 0; i < production.symbols.size(); i++) {
	if (i == position) ss << "• ";
	ss << production.symbols[i]->toS() << " ";
    }
    
    return ss.str();
}


namespace std {
    std::size_t hash<EState>::operator()(const EState& s) const {
	return s.hash();
    }
}

