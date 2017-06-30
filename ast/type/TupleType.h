//
// Created by wax on 6/27/17.
//

#ifndef DIG_TUPLE_H
#define DIG_TUPLE_H

#include <vector>
#include "TypeVisitor.h"
#include "Type.h"


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


#endif //DIG_TUPLE_H
