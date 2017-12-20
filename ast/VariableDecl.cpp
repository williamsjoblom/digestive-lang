//
// Created by wax on 12/14/16.
//

#include "util/PrettyPrint.h"
#include "gen/Gen.h"
#include "VariableDecl.h"
#include "irgen/VariableDecl.h"


VariableDecl::VariableDecl(std::string identifier, const DType type) : Decl(identifier) {
    this->value = nullptr;
    this->type = type;
}


VariableDecl::VariableDecl(std::string identifier, const DType type, Expr* value) : Decl(identifier) {
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
    }

    // Parse error should occur before assertion fails.
    assert(!this->type.isNilType() || this->value != nullptr);
    if (this->type.isNilType()) {
        this->type = this->value->type; // Infer type from value.
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


void VariableDecl::generate(TACEnv& env) {
    Generate::variableDeclaration(env, this);
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
    // FIXME: read actual stack size from type.
    return sizeof(int);
}

