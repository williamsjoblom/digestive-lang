//
// Created by wax on 2/22/17.
//

#ifndef DIG_TYPEUTIL_H
#define DIG_TYPEUTIL_H

#include "ast/type/Type.h"
#include "ast/Operator.h"

const Type* resultingType(const Type* left, Operator op, const Type* right);

#endif //DIG_TYPEUTIL_H
