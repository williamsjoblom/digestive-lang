//
// Created by wax on 12/15/16.
//

#include <util/PrettyPrint.h>
#include <gen/Gen.h>
#include "FunctionCall.h"

FunctionCall::FunctionCall(std::string identifier, std::vector<Expr*>* arguments) {
    this->identifier = identifier;
    this->arguments = arguments;
}

void FunctionCall::dump(size_t indent) {
    std::cout << identifier << "(";

    for (auto it = arguments->begin(); it != arguments->end(); it++) {
        if (it != arguments->begin())
            std::cout << ", ";
        (*it)->dump();
    }

    std::cout << ")";
}

void FunctionCall::analyze(Scope* scope) {
    declaration = scope->resolveFunction(identifier);

    for (Expr* argument : *arguments)
        argument->analyze(scope);
}

X86GpVar* FunctionCall::generate(X86Compiler &c) {
    return Generate::expression(c, this);
}
