#pragma once

#include "Type.hh"
#include "TypeVisitor.hh"

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
