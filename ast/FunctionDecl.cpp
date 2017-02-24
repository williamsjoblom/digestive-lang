//
// Created by wax on 12/14/16.
//

#include "ast/type/Types.h"
#include "util/PrettyPrint.h"
#include "gen/Gen.h"
#include "FunctionDecl.h"

FunctionDecl::FunctionDecl(std::string identifier, std::vector<VariableDecl*>* parameters, BlockStmt* body) : Decl(identifier) {
    this->parameters = parameters;
    this->body = body;

    this->baPrototype = nullptr;
}

FunctionDecl::~FunctionDecl() {
    for (VariableDecl* parameter : *parameters) delete parameter;
    delete parameters;
    delete body;
    delete baPrototype;
}

void FunctionDecl::analyze(Scope* scope) {
    // FIXME: Replace with actual type.
    this->type = &INT32TYPE;

    scope->declare(this);
    Scope* innerScope = new Scope(scope);

    for (VariableDecl* param : *this->parameters) {
        param->analyze(innerScope);
    }

    body->analyze(innerScope);
}

void FunctionDecl::dump(size_t indent) {
    printIndent(indent);
    std::cout << "func " << identifier << std::endl;
    body->dump(indent + 1);
}

void FunctionDecl::generate(X86Compiler &c) {
    Generate::function(c, this);
}

bool FunctionDecl::equals(const Node &other) const {
    const FunctionDecl* o = dynamic_cast<const FunctionDecl*>(&other);
    if (o == nullptr) return false;

    return matchesSignature(*o) && *body == *o->body;
}

bool FunctionDecl::matchesSignature(const FunctionDecl &other) const {
    if (other.parameters->size() != parameters->size()) return false;

    for (int i = 0; i < parameters->size(); i++) {
        VariableDecl* parameter = (*parameters)[i];
        VariableDecl* otherParameter = (*other.parameters)[i];

        if (*parameter != *otherParameter) return false;
    }

    return Decl::equals(other);
}


int FunctionDecl::stackSize() {
    return sizeof(void*);
}

FuncBuilderX FunctionDecl::bGetFuncPrototype() {
    if (baPrototype == nullptr) {
        baPrototype = new FuncBuilderX(kCallConvHost);
        baPrototype->setRet(TypeId<int>::kId);

        for (unsigned int i = 0; i < parameters->size(); i++) {
            baPrototype->addArg(TypeId<int>::kId);
        }
    }

    return *baPrototype;
}



