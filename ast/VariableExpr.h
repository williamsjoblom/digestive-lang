//
// Created by wax on 12/15/16.
//

#ifndef DIG_VARIABLEEXPR_H
#define DIG_VARIABLEEXPR_H

#include "gen/Base.h"
#include "Expr.h"

class VariableExpr : public Expr {
public:
    std::string identifier;
    VariableDecl* declaration;

    VariableExpr(std::string identifier);

    virtual Regs generate(X86Compiler &c);

    void analyze(Scope* scope);

    virtual bool equals(const Node& other) const;

    void dump(size_t indent);

};


#endif //DIG_VARIABLEEXPR_H
