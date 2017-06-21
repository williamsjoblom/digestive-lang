//
// Created by wax on 12/14/16.
//

#include <util/PrettyPrint.h>
#include <gen/Gen.h>
#include "VariableDecl.h"

VariableDecl::VariableDecl(std::string identifier) : Decl(identifier) {
    this->value = nullptr;
}

VariableDecl::VariableDecl(std::string identifier, Expr* value) : Decl(identifier) {
    this->value = value;
}

VariableDecl::~VariableDecl() {
    delete value;
}

void VariableDecl::analyze(Scope* scope) {
    scope->declare(this);

    if (this->value != nullptr) {
        this->value->analyze(scope);
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
    if (o->value == nullptr && value == nullptr) return Decl::equals(other);

    return *o->value == *value && Decl::equals(other);
}

int VariableDecl::stackSize() {
    return sizeof(int);
}

