//
// Created by wax on 2/22/17.
//

#ifndef DIG_PRIMITIVETYPE_H
#define DIG_PRIMITIVETYPE_H


#include "Type.h"
#include "TypeVisitor.h"

enum class PrimitiveKind {
    INTEGER,
    REAL
};

class PrimitiveType : public Type {
public:
    PrimitiveKind kind;
    int size;
    PrimitiveType() = default;
    PrimitiveType(PrimitiveKind kind, int size);

    bool isInt() override;
    bool isFloat() override;

    bool equals(const Type &other) const override;

    void accept(TypeVisitor* visitor) const { visitor->visit(this); }
    void dump() const;
};


#endif //DIG_PRIMITIVETYPE_H
