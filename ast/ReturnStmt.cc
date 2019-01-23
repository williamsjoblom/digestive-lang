#include "ReturnStmt.hh"

#include "ast/type/Types.hh"
#include "util/PrettyPrint.hh"
#include "genir/Return.hh"


ReturnStmt::ReturnStmt(Expr* expression) : expression{expression} { }


ReturnStmt::~ReturnStmt() {
    delete expression;
}


void ReturnStmt::dump(size_t indent) {
    printIndent(indent);
    std::cout << "return ";
    expression->dump(indent);
    std::cout << std::endl;
}


void ReturnStmt::analyze(Scope* scope) {
    expression->analyze(scope);

    if (scope->functionContext != nullptr)
        returnType = scope->functionContext->returnType;
    else
        returnType = N8_TYPE; // No function context <=> returns exit code.
}


void ReturnStmt::generate(TACFun* fun) {
    Generate::returnStmt(fun, this);
}


bool ReturnStmt::equals(const Node &other) const {
    const ReturnStmt* o { dynamic_cast<const ReturnStmt*>(&other) };
    if (o == nullptr) return false;

    return *o->expression == *expression;
}


