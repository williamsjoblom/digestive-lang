//
// Created by wax on 12/14/16.
//

#include <util/PrettyPrint.h>
#include <gen/Gen.h>
#include <ast/type/Types.h>
#include <semantic/SemanticError.h>
#include "VariableDecl.h"

VariableDecl::VariableDecl(std::string identifier, Type* type) : Decl(identifier) {
    this->value = nullptr;
    this->type = type;
}

VariableDecl::VariableDecl(std::string identifier, Type* type, Expr* value) : Decl(identifier) {
    this->value = value;
    this->type = type;
}

VariableDecl::~VariableDecl() {
    delete value;
}

void VariableDecl::analyze(Scope* scope) {
    scope->declare(this);

    if (this->value != nullptr) {
        this->value->analyze(scope);

        // Type inference
        if (this->type == nullptr) {
            this->type = value->type;
        } else if (*this->type != *value->type) {
            // TODO: Check if an implicit cast is possible.
            semanticError("Type mismatch");
            throw 1;
        }
    }
}

void VariableDecl::dump(size_t indent) {
    printIndent(indent);
    std::cout << "var " << identifier;

    if(value != nullptr)
        std::cout << " = ";
    value->dump(indent + 1);

    std::cout << std::endl;
}

void VariableDecl::generate(X86Compiler &c) {
    Generate::variableDeclaration(c, this);
}

bool VariableDecl::equals(const Node &other) const {
    const VariableDecl* o = dynamic_cast<const VariableDecl*>(&other);
    if (o == nullptr) return false;

    if (o->value == nullptr && value != nullptr) return false;
    if (o->value != nullptr && value == nullptr) return false;

    return *o->value == *value && Decl::equals(other);
}

int VariableDecl::stackSize() {
    return sizeof(int);
}

