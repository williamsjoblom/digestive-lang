//
// Created by wax on 12/14/16.
//

#ifndef DIG_VARIABLEDECL_H
#define DIG_VARIABLEDECL_H

#include "Decl.h"
#include "Expr.h"

class VariableDecl : public Decl {
public:
    Expr* value;
    X86Var* bVar;

    VariableDecl(std::string identifier, Type* type);
    VariableDecl(std::string identifier, Type* type, Expr* value);
    ~VariableDecl();

    virtual void generate(X86Compiler &c);

    void analyze(Scope* scope);
    virtual bool equals(const Node& other) const;
    void dump(size_t indent);

    int stackSize() override;
};

#endif //DIG_VARIABLEDECL_H
