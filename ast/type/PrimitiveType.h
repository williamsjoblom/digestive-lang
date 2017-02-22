//
// Created by wax on 2/22/17.
//

#ifndef DIG_PRIMITIVETYPE_H
#define DIG_PRIMITIVETYPE_H


#include "Type.h"

enum class PrimitiveKind {
    INTEGER,
    REAL
};

class PrimitiveType : public Type {
public:
    PrimitiveKind kind;
    int size;
    PrimitiveType(PrimitiveKind kind, int size);
};


#endif //DIG_PRIMITIVETYPE_H
