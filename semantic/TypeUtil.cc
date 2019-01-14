#include "ast/type/PrimitiveType.hh"
#include "ast/type/DType.hh"
#include "TypeUtil.hh"
#include "ast/type/TypeVisitor.hh"
#include "SemanticError.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/VariableExpr.hh"


const DType resultingPrimitive(const DType& left, Operator op, const DType& right) {
    assert(left.isPrimitive() && right.isPrimitive());

    if (left.type.primitive == DPrimitiveKind::NIL || right.type.primitive == DPrimitiveKind::NIL)
        semanticError("Nil types cannot be part of expressions");

    int sz = std::max(left.byteSize(), right.byteSize());
    DPrimitiveKind kind;
    if (left.type.primitive == DPrimitiveKind::NATURAL && right.type.primitive == DPrimitiveKind::NATURAL) {
        kind = DPrimitiveKind::NATURAL;
    } else {
        kind = DPrimitiveKind::INTEGER;
    }

    return DType(kind, sz);
}


const DType resultingTuplePrimitive(const Expr* left, Operator op, const Expr* right) {
    if (op.symbol == OperatorSymbol::DOT) {

	std::vector<DType> tupleTypes = *left->type.type.tuple;
	
	const IntegerLiteral* literal = dynamic_cast<const IntegerLiteral*>(right);
	if (literal != nullptr) {
	    int i = literal->value;
	    if (i >= tupleTypes.size()) semanticError("Tuple index out of bounds");

	    DType type = tupleTypes[i];
	    type.ref = true;
	    return type;
	}

	const VariableExpr* label = dynamic_cast<const VariableExpr*>(right);
	if (label != nullptr) {
	    std::string identifier = label->identifier;

	    for (DType type : tupleTypes) {
		if (type.label == identifier) {
		    type.ref = true;
		    return type;
		}
	    }

	    semanticError("No such tuple member");
	}

	semanticError("Tuples are only indexable by integer literals or names");
    }

    semanticError("Bad type conversion");
}

const DType resultingAssign(const Expr* left, Operator op, const Expr* right) {
    if (left->type.isTuple() && right->type.isPrimitive())
	semanticError("Cannot assign primitive to tuple");
    if (left->type.isTuple() && right->type.isTuple()) {
	std::vector<DType> leftTypes = *left->type.type.tuple;
	std::vector<DType> rightTypes = *right->type.type.tuple;
	if (leftTypes.size() > rightTypes.size())
	    semanticError("Bad tuple assignment");
	
	for (int i = 0; i < leftTypes.size(); i++) {
	    DType l = leftTypes[i];
	    DType r = rightTypes[i];
	    if (l != r) semanticError("Bad tuple assignment");
	}
	
    }

    return left->type;
}

const DType resultingType(Expr* left, Operator op, Expr* right) {
    if (op.symbol == OperatorSymbol::ASSIGN) return resultingAssign(left, op, right);
    
    if (left->type.isPrimitive() && right->type.isPrimitive()) return resultingPrimitive(left->type, op, right->type);
    if (left->type.isTuple()) return resultingTuplePrimitive(left, op, right);

    semanticError("Bad type conversion");
}
