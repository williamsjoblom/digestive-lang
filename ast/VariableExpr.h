//
// Created by wax on 12/15/16.
//

#ifndef DIG_VARIABLEEXPR_H
#define DIG_VARIABLEEXPR_H


#include "Expr.h"

class VariableExpr : public Expr {
public:
    std::string identifier;

    VariableExpr(std::string identifier);

    void dump(size_t indent);

};


#endif //DIG_VARIABLEEXPR_H
