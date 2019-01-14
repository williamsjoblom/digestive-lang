#pragma once

#include "semantic/Scope.hh"

class Scope;

class Analyzable {
public:
    virtual void analyze(Scope* scope) = 0;
};
