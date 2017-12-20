//
// Created by wax on 12/15/16.
//

#ifndef DIG_FUNCTIONCALL_H
#define DIG_FUNCTIONCALL_H

#include "Expr.h"
#include "gen/Base.h"

class FunctionCall : public Expr {
public:
    std::string identifier;
    std::vector<Expr*>* arguments;
    FunctionDecl* declaration;

    FunctionCall(std::string identifier, std::vector<Expr*>* arguments);
    ~FunctionCall();

    TACOp generate(TACEnv& env);
    Regs generate(X86Compiler &c);

    void analyze(Scope* scope);

    bool equals(const Node& other) const;

    void dump(size_t indent);
};


#endif //DIG_FUNCTIONCALL_H
