//
// Created by wax on 2/22/17.
//

#ifndef DIG_TYPEVISITOR_H
#define DIG_TYPEVISITOR_H

class PrimitiveType;

class TypeVisitor {
public:
    TypeVisitor() = default;

    virtual void visit(const PrimitiveType* type) = 0;
};

#endif //DIG_TYPEVISITOR_H
