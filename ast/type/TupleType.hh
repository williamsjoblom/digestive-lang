#pragma once

#include <vector>

#include "TypeVisitor.hh"
#include "Type.hh"


class TupleType : public Type {
public:
    std::vector<const Type*> types;

    TupleType(std::vector<const Type*> types);

    virtual int byteSize() const override;
    virtual bool isTuple() const { return true; }
    virtual void accept(TypeVisitor* visitor) const { visitor->visit(this); }
    virtual void dump() const override;
    virtual bool equals(const Node& other) const;
};
