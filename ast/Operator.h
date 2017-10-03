//
// Created by wax on 12/15/16.
//

#ifndef DIG_OPERATOR_H
#define DIG_OPERATOR_H

#include <string>
#include "Node.h"

enum class OperatorSymbol {
    PLUS,
    MINUS,
    MUL,
    DIV,

    ASSIGN,

    EQ,
    NOTEQ,
    LESSEQ,
    GREATEREQ,
    LESS,
    GREATER,
    DOT,

    NOT_AN_OPERATOR
};

class Operator : public Node {
public:
    OperatorSymbol symbol;
    int precedence;

    Operator();
    Operator(std::string s);
    Operator(OperatorSymbol symbol);

    static bool isOperator(std::string s);

    bool isBinary();
    bool isUnary();
    bool isRightAssociative();
    bool isLeftAssociative();

    void analyze(Scope* scope) { }
    virtual bool equals(const Node& other) const;

    void dump(size_t indent);
};


#endif //DIG_OPERATOR_H
