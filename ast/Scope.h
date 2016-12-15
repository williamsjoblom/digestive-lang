//
// Created by wax on 12/15/16.
//

#ifndef DIG_SCOPE_H
#define DIG_SCOPE_H


#include <map>
#include "VariableDecl.h"
#include "FunctionDecl.h"

class Scope {
public:
    Scope* parent;

    Scope();
    Scope(Scope* parent);

    void declare(VariableDecl* variableDecl);
    void declare(FunctionDecl* functionDecl);

    VariableDecl* resolveVariable(std::string identifier);
    FunctionDecl* resolveFunction(std::string identifier);

private:
    std::map<std::string, VariableDecl> variables;
    std::map<std::string, FunctionDecl> functions;
};


#endif //DIG_SCOPE_H
