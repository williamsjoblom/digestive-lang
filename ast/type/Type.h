//
// Created by wax on 2/22/17.
//

#ifndef DIG_TYPE_H
#define DIG_TYPE_H


class TypeVisitor;

class Type {
public:
    Type() = default;

    virtual void accept(TypeVisitor* visitor) const = 0;
    virtual void dump() const = 0;
};


#endif //DIG_TYPE_H
