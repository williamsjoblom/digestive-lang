#pragma once

#include "Decl.hh"
#include "ast/type/DType.hh"

/**
 * Forward declarations.
 */
class Expr;
class TACVar;

class VariableDecl : public Decl {
public:
    Expr* value;
    DType type;
    TACVar* irVar;

    VariableDecl(std::string identifier, DType type);
    VariableDecl(std::string identifier, DType type, Expr* value);
    ~VariableDecl();

    void generate(TACFun* fun) override;

    void analyze(Scope* scope) override;
    virtual bool equals(const Node& other) const override;
    void dump(size_t indent) override;
   

    int stackSize() override;
};
