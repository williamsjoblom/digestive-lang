#pragma once

#include "Expr.hh"

class TACFun;

class TupleExpr : public Expr {
public:

    std::vector<Expr*> values;

    TupleExpr(std::vector<Expr*> values);
    ~TupleExpr();

    TACOp generate(TACFun* fun);
    
    void analyze(Scope* scope);

    bool equals(const Node& other) const;

    void dump(size_t indent);
};
