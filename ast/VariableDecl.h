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
    X86GpVar* bVar;

    VariableDecl(std::string identifier);
    VariableDecl(std::string identifier, Expr* value);

    virtual void generate(X86Compiler &c);

    void analyze(Scope* scope);

    void dump(size_t indent);
};

#endif //DIG_VARIABLEDECL_H
