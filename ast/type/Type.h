//
// Created by wax on 2/22/17.
//

#ifndef DIG_TYPE_H
#define DIG_TYPE_H


#include <assert.h>

class TypeVisitor;

class Type {
public:
    Type() = default;

    virtual bool isInt() { return false; };
    virtual bool isFloat() { return false; };

    virtual void accept(TypeVisitor* visitor) const = 0;
    virtual void dump() const = 0;

    bool operator==(const Type& other) { return equals(other); }
    bool operator!=(const Type& other) const { return !equals(other); }
    virtual bool equals(const Type& other) const { assert(false); }
};


#endif //DIG_TYPE_H
