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

                {"==", OperatorSymbol::EQ},
                {"!=", OperatorSymbol::NOTEQ},
                {"<=", OperatorSymbol::LESSEQ},
                {">=", OperatorSymbol::GREATEREQ},
                {"<", OperatorSymbol::LESS},
                {">", OperatorSymbol::GREATER},
        };

bool Operator::isOperator(std::string s) {
    return stringToSymbol.find(s) != stringToSymbol.end();
}

bool Operator::equals(const Node &other) const {
    const Operator* o = dynamic_cast<const Operator*>(&other);
    if (o == nullptr) return false;

    return o->symbol == symbol;
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

        case OperatorSymbol::EQ: return "==";
        case OperatorSymbol::NOTEQ: return "!=";
        case OperatorSymbol::LESSEQ: return "<=";
        case OperatorSymbol::GREATEREQ: return ">=";
        case OperatorSymbol::LESS: return "<";
        case OperatorSymbol::GREATER: return ">";
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

        case OperatorSymbol::EQ:
        case OperatorSymbol::NOTEQ:
        case OperatorSymbol::LESSEQ:
        case OperatorSymbol::GREATEREQ:
        case OperatorSymbol::LESS:
        case OperatorSymbol::GREATER:
            return 100;
    }

    assert(false);
}