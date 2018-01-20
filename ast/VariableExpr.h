//
// Created by wax on 12/15/16.
//

#ifndef DIG_VARIABLEEXPR_H
#define DIG_VARIABLEEXPR_H

#include "Expr.h"

class VariableExpr : public Expr {
public:
    std::string identifier;
    VariableDecl* declaration;

    VariableExpr(std::string identifier);

    virtual TACOp generate(TACFun* env) override;

    void analyze(Scope* scope) override;

    virtual bool equals(const Node& other) const override;

    void dump(size_t indent) override;

};


#endif //DIG_VARIABLEEXPR_H
