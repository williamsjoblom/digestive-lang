//
// Created by wax on 12/15/16.
//

#include <gen/Gen.h>
#include <ast/type/Types.h>
#include "util/PrettyPrint.h"
#include "ReturnStmt.h"

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

void ReturnStmt::generate(X86Compiler &c) {
    Generate::statement(c, this);
}

bool ReturnStmt::equals(const Node &other) const {
    const ReturnStmt* o = dynamic_cast<const ReturnStmt*>(&other);
    if (o == nullptr) return false;

    return *o->expression == *expression;
}


