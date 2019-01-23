#include "VariableExpr.hh"

#include "util/PrettyPrint.hh"
#include "genir/Expr.hh"
#include "ir/TACFun.hh"

VariableExpr::VariableExpr(std::string identifier) : identifier{identifier} { }


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


bool VariableExpr::equals(const Node &other) const {
    const VariableExpr* o { dynamic_cast<const VariableExpr*>(&other) };
    if (o == nullptr) return false;

    return o->identifier == identifier;
}
