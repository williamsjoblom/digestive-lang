//
// Created by wax on 12/15/16.
//

#include "Scope.h"

Scope::Scope() {
    this->parent = nullptr;
}

Scope::Scope(Scope* parent) {
    this->parent = parent;
}

void Scope::declare(VariableDecl* variableDecl) {
    
}

void Scope::declare(FunctionDecl* functionDecl) {

}

VariableDecl *Scope::resolveVariable(std::string identifier) {
    return nullptr;
}

FunctionDecl *Scope::resolveFunction(std::string identifier) {
    return nullptr;
}
