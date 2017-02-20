//
// Created by wax on 12/15/16.
//

#include "gen/Gen.h"
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

X86GpVar VariableExpr::generate(X86Compiler &c) {
    return Generate::expression(c, this);
}

bool VariableExpr::equals(const Node &other) const {
    const VariableExpr* o = dynamic_cast<const VariableExpr*>(&other);
    if (o == nullptr) return false;

    return o->identifier == identifier;
}
