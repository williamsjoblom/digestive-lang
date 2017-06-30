//
// Created by wax on 12/14/16.
//

#include "util/PrettyPrint.h"
#include "gen/Gen.h"
#include "FunctionDecl.h"

FunctionDecl::FunctionDecl(std::string identifier, std::vector<VariableDecl*>* parameters, BlockStmt* body,
                           const Type* returnType, bool dumpAssembly) : Decl(identifier) {
    this->parameters = parameters;
    this->body = body;
    this->bHandleIndex = -1;
    this->codeSize = -1;
    this->returnType = returnType;
    this->dumpAssembly = dumpAssembly;

    this->baPrototype = nullptr;
}

FunctionDecl::~FunctionDecl() {
    for (VariableDecl* parameter : *parameters) delete parameter;
    delete parameters;
    delete body;
    delete baPrototype;
}

void FunctionDecl::analyze(Scope* scope) {
    scope->declare(this);
    Scope* innerScope = new Scope(scope, this);

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

FuncSignatureX FunctionDecl::bCreatePrototype() {
    if (baPrototype == nullptr) {
        baPrototype = new FuncSignatureX(CallConv::kIdHostCDecl);
        baPrototype->setRet(TypeIdOf<int>::kTypeId);

        for (unsigned int i = 0; i < parameters->size(); i++) {
            baPrototype->addArg(TypeIdOf<int>::kTypeId);
        }
    }

    return *baPrototype;
}



