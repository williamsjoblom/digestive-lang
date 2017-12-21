//
// Created by wax on 12/14/16.
//

#ifndef DIG_VARIABLEDECL_H
#define DIG_VARIABLEDECL_H

#include "gen/Gen.h"
#include "Decl.h"

/**
 * Forward declarations.
 */
class Expr;
class TACVar;

class VariableDecl : public Decl {
public:
    Expr* value;
    DType type;
    Regs bVar;
    TACVar* irVar;

    VariableDecl(std::string identifier, DType type);
    VariableDecl(std::string identifier, DType type, Expr* value);
    ~VariableDecl();

    void generate(X86Compiler& c) override;
    void generate(TACFun* env) override;

    void analyze(Scope* scope) override;
    virtual bool equals(const Node& other) const override;
    void dump(size_t indent) override;
   

    int stackSize() override;
};

#endif //DIG_VARIABLEDECL_H
