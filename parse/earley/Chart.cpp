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


bool addState(EChart* chart, EState state, int k) {
    hash_t hash = state.hash();
    if (!chart->contains(state, k)) {
	chart->sh[k].insert(hash);
	chart->s[k].insert(chart->s[k].end(), state);
	return true;
    }

    return false;
}


bool EChart::add(BNFGrammar& g, EState state, int k) {
    bool changed = addState(this, state, k);

    // TODO Move to Parse.cpp:
    if (!state.complete() &&
	state.next()->nullable(g)) {
	EState s(state);
	s.position++;
	changed |= addState(this, s, k);
    }

    return changed;
}


std::list<EState>& EChart::set(int k) {
    return s[k];
}


bool EChart::contains(EState& state, int k) {
    hash_t hash = state.hash();
    return sh[k].find(hash) != sh[k].end();
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
    return
	std::hash<std::string>()(symbol) ^ production.hash() ^
	orderedHash({origin, position});
}


bool EState::operator==(const EState& other) const {
    return hash() == other.hash();
}


std::string EState::toS() const {
    std::stringstream ss;

    ss << "(" << symbol << " →";
    
    for (int i = 0; i < production.symbols.size(); i++) {
	if (i == position) ss << " •";
	ss << ' ' << production.symbols[i]->toS();
    }

    if (position == production.symbols.size())
	ss << " •";

    ss << ", " << origin << ")";

    if (!production.nodeLabel.empty())
	ss << " @" << production.nodeLabel;

    ss << " (" << message << ")";
    
    return ss.str();
}


namespace std {
    std::size_t hash<EState>::operator()(const EState& s) const {
	return s.hash();
    }
}

