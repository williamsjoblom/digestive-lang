//
// Created by wax on 12/15/16.
//

#include <util/PrettyPrint.h>
#include "Scope.h"
#include "SemanticError.h"

#include "ast/FunctionDecl.h"
#include "ast/VariableDecl.h"

Scope::Scope(Scope* parent, FunctionDecl* functionContext) {
    this->parent = parent;

    if (functionContext == nullptr && parent != nullptr)
        this->functionContext = parent->functionContext;
    else
        this->functionContext = functionContext;

    if (parent != nullptr)
        parent->addChild(this);
}

Scope::~Scope() {
    if (parent != nullptr)
        parent->removeChild(this);

    for (auto it = children.begin(); it != children.end(); it++)
        delete *it;
}

void Scope::declare(VariableDecl* variableDecl) {
    if(contains(variableDecl->identifier))
        semanticError("Redeclaration of variable '" + variableDecl->identifier + "'");
    variables[variableDecl->identifier] = variableDecl;
}

void Scope::declare(FunctionDecl* functionDecl) {
    if(contains(functionDecl->identifier))
        semanticError("Redeclaration of function '" + functionDecl->identifier + "'");
    functions[functionDecl->identifier] = functionDecl;
}

bool Scope::contains(std::string identifier) {
    if (variables.find(identifier) != variables.end()) return true;
    return functions.find(identifier) != functions.end();
}

void Scope::addChild(Scope* scope) {
    children.insert(scope);
}

void Scope::removeChild(Scope* scope) {
    children.erase(scope);
}

VariableDecl* Scope::resolveVariable(std::string identifier) {
    if(variables.find(identifier) != variables.end()) {
        return variables[identifier];
    }

    if (parent == nullptr)
        semanticError("Variable '" + identifier + "' was not declared in this scope");

    return parent->resolveVariable(identifier);
}

FunctionDecl* Scope::resolveFunction(std::string identifier) {
    if(functions.find(identifier) != functions.end()) {
        return functions[identifier];
    }

    if (parent == nullptr)
        semanticError("Function '" + identifier + "' was not declared in this scope");

    return parent->resolveFunction(identifier);
}

void Scope::dump(size_t indent) {
    for (auto it = functions.begin(); it != functions.end(); it++) {
        printIndent(indent);
        std::cout << "func " << (*it).first << std::endl;
    }

    for (auto it = variables.begin(); it != variables.end(); it++) {
        printIndent(indent);
        std::cout << "var " << (*it).first << std::endl;
    }

    if (!children.empty()) {
        printIndent(indent);
        std::cout << "{" << std::endl;

        for (Scope *child : children)
            child->dump(indent + 1);

        printIndent(indent);
        std::cout << "}" << std::endl;
    }
}

int Scope::stackSize() {
    return 0;
}


