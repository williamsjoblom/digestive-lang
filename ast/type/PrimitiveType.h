//
// Created by wax on 2/22/17.
//

#ifndef DIG_PRIMITIVETYPE_H
#define DIG_PRIMITIVETYPE_H


#include "Type.h"
#include "TypeVisitor.h"

enum PrimitiveKind {
    VOID,
    INTEGER,
    NATURAL
};

class PrimitiveType : public Type {
public:
    PrimitiveKind kind;
    int sz;
    PrimitiveType(PrimitiveKind kind, int size);

    virtual int byteSize() const override;
    virtual bool isPrimitive() const override;
    virtual void accept(TypeVisitor* visitor) const { visitor->visit(this); }
    virtual void dump() const override;
    bool equals(const Node &other) const;
};


#endif //DIG_PRIMITIVETYPE_H
