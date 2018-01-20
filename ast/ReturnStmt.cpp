#include "ReturnStmt.h"

#include "ast/type/Types.h"
#include "util/PrettyPrint.h"
#include "genir/Return.h"


ReturnStmt::ReturnStmt(Expr* expression) {
    this->expression = expression;
}


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
    const ReturnStmt* o = dynamic_cast<const ReturnStmt*>(&other);
    if (o == nullptr) return false;

    return *o->expression == *expression;
}


