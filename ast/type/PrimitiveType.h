//
// Created by wax on 2/22/17.
//

#ifndef DIG_PRIMITIVETYPE_H
#define DIG_PRIMITIVETYPE_H


#include "Type.h"
#include "TypeVisitor.h"

enum PrimitiveKind {
    INTEGER,
    REAL
};

class PrimitiveType : public Type {
public:
    PrimitiveKind kind;
    int size;
    PrimitiveType(PrimitiveKind kind, int size);

    void accept(TypeVisitor* visitor) const { visitor->visit(this); }
    void dump() const;
};


#endif //DIG_PRIMITIVETYPE_H
