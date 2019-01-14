#pragma once

#include "Expr.hh"

class Literal : public Expr {
public:
    Literal() = default;

    void analyze(Scope* scope) { }
};
