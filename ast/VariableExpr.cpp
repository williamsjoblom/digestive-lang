//
// Created by wax on 12/15/16.
//

#include "VariableExpr.h"

#include "gen/Gen.h"
#include "util/PrettyPrint.h"
#include "irgen/Expr.h"


VariableExpr::VariableExpr(std::string identifier) {
    this->identifier = identifier;
}


void VariableExpr::analyze(Scope* scope) {
    declaration = scope->resolveVariable(identifier);
    type = declaration->type;
}


void VariableExpr::dump(size_t indent) {
    printIndent(indent);
    std::cout << identifier << std::endl;
}


TACOp VariableExpr::generate(TACFun* env) {
    return Generate::variable(env, this);
}


Regs VariableExpr::generate(X86Compiler& c) {
    return Generate::expression(c, this);
}


bool VariableExpr::equals(const Node &other) const {
    const VariableExpr* o = dynamic_cast<const VariableExpr*>(&other);
    if (o == nullptr) return false;

    return o->identifier == identifier;
}
