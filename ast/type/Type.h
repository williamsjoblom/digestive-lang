//
// Created by wax on 2/22/17.
//

#ifndef DIG_TYPE_H
#define DIG_TYPE_H


#include "ast/Node.h"

class TypeVisitor;

class Type : public Node {
public:
    Type() = default;

    virtual int byteSize() const = 0;
    virtual bool isPrimitive() const { return false; }
    virtual bool isTuple() const { return false; }
    virtual void accept(TypeVisitor* visitor) const = 0;
    virtual void dump() const = 0;
    virtual void dump(size_t indent) { dump(); };
    void analyze(Scope* scope) { }
};


#endif //DIG_TYPE_H
