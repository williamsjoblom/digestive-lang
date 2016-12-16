//
// Created by wax on 12/15/16.
//

#ifndef DIG_ANALYZABLE_H
#define DIG_ANALYZABLE_H

#include "semantic/Scope.h"

class Scope;

class Analyzable {
public:
    virtual void analyze(Scope* scope) = 0;
};

#endif //DIG_ANALYZABLE_H
