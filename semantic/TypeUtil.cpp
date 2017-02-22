//
// Created by wax on 2/22/17.
//

#include <ast/type/PrimitiveType.h>
#include "TypeUtil.h"
#include "ast/type/TypeVisitor.h"

class TypeResultVisitor : TypeVisitor {
public:
    Type* resultingType(Type* type, OperatorSymbol op, Type* other);

private:
    Type* result;

    Type* type;
    OperatorSymbol op;
    Type* other;

    void visit(const PrimitiveType* type) override;
};

Type* TypeResultVisitor::resultingType(Type* type, OperatorSymbol op, Type* other)  {
    this->op = op;
    this->other = other;

    this->result = nullptr;

    type->accept(this);
    return this->result;
}

void TypeResultVisitor::visit(const PrimitiveType* type) {
    const PrimitiveType* other = dynamic_cast<const PrimitiveType*>(this->other);
    if (other == nullptr) return;

    PrimitiveKind kind;
    int size;

    if (type->kind == PrimitiveKind::REAL || other->kind == PrimitiveKind::REAL)
        kind = PrimitiveKind::REAL;
    else
        kind = PrimitiveKind::INTEGER;

    size = std::max(type->size, other->size);

    result = new PrimitiveType(kind, size);
}

Type* resultingType(Type* left, Operator op, Type* right) {
    // TODO: Maybe check for operator associativity?
    // if left associative:
    TypeResultVisitor resultChecker;
    return resultChecker.resultingType(left, op.symbol, right);
    // if right associative:
    // return resultChecker.resultingType(right, op.symbol, left);
}