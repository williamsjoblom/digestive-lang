//
// Created by wax on 2/22/17.
//

#ifndef DIG_TYPEUTIL_H
#define DIG_TYPEUTIL_H

#include "ast/type/Type.h"
#include "ast/Operator.h"

Type* resultingType(Type* left, Operator op, Type* right);

#endif //DIG_TYPEUTIL_H
