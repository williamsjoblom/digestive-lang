//
// Created by wax on 2/22/17.
//

#ifndef DIG_TYPEUTIL_H
#define DIG_TYPEUTIL_H

#include "ast/type/Type.h"
#include "ast/Operator.h"
#include "ast/Expr.h"

const DType resultingType(Expr* left, Operator op, Expr* right);

#endif //DIG_TYPEUTIL_H
