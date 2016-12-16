//
// Created by wax on 12/15/16.
//

#include "util/PrettyPrint.h"
#include "VariableExpr.h"

VariableExpr::VariableExpr(std::string identifier) {
    this->identifier = identifier;
}

void VariableExpr::analyze(Scope* scope) {
    declaration = scope->resolveVariable(identifier);
}

void VariableExpr::dump(size_t indent) {
    printIndent(indent);
    std::cout << identifier << std::endl;
}


