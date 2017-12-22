//
// Created by wax on 6/30/17.
//

#ifndef DIG_TUPLEEXPR_H
#define DIG_TUPLEEXPR_H

#include "gen/Base.h"
#include "Expr.h"

class TACFun;

class TupleExpr : public Expr {
public:

    std::vector<Expr*> values;

    TupleExpr(std::vector<Expr*> values);
    ~TupleExpr();

    TACOp generate(TACFun* fun);
    Regs generate(X86Compiler& c);

    void analyze(Scope* scope);

    bool equals(const Node& other) const;

    void dump(size_t indent);
};


#endif //DIG_TUPLEEXPR_H
