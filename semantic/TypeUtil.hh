#pragma once

#include "ast/type/Type.hh"
#include "ast/Operator.hh"
#include "ast/Expr.hh"

const DType resultingType(Expr* left, Operator op, Expr* right);
