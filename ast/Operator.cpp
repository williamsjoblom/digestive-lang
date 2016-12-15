//
// Created by wax on 12/15/16.
//

#include "Operator.h"

#include <map>
#include <assert.h>
#include <util/PrettyPrint.h>

OperatorSymbol symbolFromString(std::string s);
const std::string symbolToString(OperatorSymbol symbol);
int precedenceOf(OperatorSymbol symbol);

Operator::Operator(std::string s) {
    this->symbol = symbolFromString(s);
    this->precedence = precedenceOf(symbol);
}

Operator::Operator(OperatorSymbol symbol) {
    this->symbol = symbol;
    this->precedence = precedenceOf(symbol);
}

void Operator::dump(size_t indent) {
    std::cout << symbolToString(this->symbol);
}

const std::map<std::string, OperatorSymbol> stringToSymbol =
        {
                {"+", OperatorSymbol::PLUS},
                {"-", OperatorSymbol::MINUS},
                {"*", OperatorSymbol::MUL},
                {"/", OperatorSymbol::DIV},
        };

bool Operator::isOperator(std::string s) {
    return stringToSymbol.find(s) != stringToSymbol.end();
}

OperatorSymbol symbolFromString(std::string s) {
    return stringToSymbol.at(s);
}

const std::string symbolToString(OperatorSymbol symbol) {
    switch(symbol) {
        case OperatorSymbol::PLUS: return "+";
        case OperatorSymbol::MINUS: return "-";
        case OperatorSymbol::MUL: return "*";
        case OperatorSymbol::DIV: return "/";
    }

    assert(false);
}

int precedenceOf(OperatorSymbol symbol) {
    switch (symbol) {
        case OperatorSymbol::PLUS:
        case OperatorSymbol::MINUS:
            return 10;
        case OperatorSymbol::MUL:
        case OperatorSymbol::DIV:
            return 20;
    }

    assert(false);
}