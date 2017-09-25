//
// Created by wax on 6/27/17.
//

#include <asmjit/asmjit.h>
#include <assert.h>
#include <ast/type/PrimitiveType.h>

#include "Gen.h"
#include "CallConv.h"

using namespace asmjit;


/*
 * Generate primitive primitive binary expression.
 */
X86Gp primitiveBinaryExpr(X86Compiler &c, BinaryExpr* expr) {
    DType& type = expr->type;
    assert(!type.isNilType());

    Regs leftRegs = Generate::cast(c, expr->left, type);
    Regs rightRegs = Generate::cast(c, expr->right, type);
    assert(rightRegs.size() == 1 && leftRegs.size() == 1);

    X86Gp left = leftRegs[0];
    X86Gp right = rightRegs[0];

    assert(left.getSize() == right.getSize());

    Regs resultRegs = Generate::typedRegister(c, type);
    assert(resultRegs.size() == 1);
    X86Gp result = resultRegs[0];

    switch(expr->op->symbol) {
        case OperatorSymbol::PLUS:
            c.mov(result, left);
            c.add(result, right);
            break;
        case OperatorSymbol::MINUS:
            c.mov(result, left);
            c.sub(result, right);
            break;
        case OperatorSymbol::MUL:
            c.mov(result, left);
            if (type.type.primitive == DPrimitiveKind::INTEGER)
                c.imul(result, right);
            else
                c.mul(result, right);
            break;
        case OperatorSymbol::DIV:
            c.mov(result, left);
            assert(false); // Not implemented.
            break;

        case OperatorSymbol::EQ:
            c.cmp(left, right);
            c.sete(result.r8());
            c.movzx(result, result.r8());
            break;
        case OperatorSymbol::NOTEQ:
            c.cmp(left, right);
            c.setne(result.r8());
            c.movzx(result, result.r8());
            break;
        case OperatorSymbol::LESSEQ:
            c.cmp(left, right);
            c.setle(result.r8());
            c.movzx(result, result.r8());
            break;
        case OperatorSymbol::GREATEREQ:
            c.cmp(left, right);
            c.setge(result.r8());
            c.movzx(result, result.r8());
            break;
        case OperatorSymbol::LESS:
            c.cmp(left, right);
            c.setl(result.r8());
            c.movzx(result, result.r8());
            break;
        case OperatorSymbol::GREATER:
            c.cmp(left, right);
            c.setg(result.r8());
            c.movzx(result, result.r8());
            break;
        default:
            assert(false);
    }

    return result;
}


/*
 * Generate tuple access expression: (a, b, c).2
 */
Regs tupleAccessExpr(X86Compiler& c, BinaryExpr* expr) {
    assert(expr->left->type.isTuple());
    assert(expr->right->type.isPrimitive());
    assert(expr->op->symbol == OperatorSymbol::DOT);

    //assert(!expr->type.isTuple()); // Not implemented. 
    
    IntegerLiteral* literal = dynamic_cast<IntegerLiteral*>(expr->right);

    std::vector<DType>* containedTypes = expr->left->type.type.tuple;
    
    int index = 0;
    for (int i = 0; i < literal->value; i++) {
	DType& containedType = containedTypes->at(i);
	std::cout << "Contained type = " << std::endl;
	containedType.dump();
	std::cout << std::endl;
	
	index += neededRegisterCount(containedType);
	std::cout << "index = " << index << std::endl;
	
	/*
	if (containedType.isPrimitive())
	    index++;
	else if (containedType.isTuple())
	    index += containedType.type.tuple->size();
	else
	    assert(false); // Not implemented.
	*/
    }
    
    Regs leftRegs = expr->left->generate(c);
	
    if (expr->type.isTuple()) {
	Regs result;
	
	int size = neededRegisterCount(expr->type);
	for (int i = 0; i < size; i++) {
	    X86Gp reg = leftRegs[index + i];
	    result.push_back(reg);
	}

	std::cout << "Tuple access tuple; index, size = " << index << " "
		  << result.size() << std::endl;
	expr->type.dump();
	std::cout << std::endl << std::endl;
    } else {
	X86Gp reg = leftRegs[index];
	std::cout << "tuple access primitive " << index << std::endl;
	return { reg };
    }
}

namespace Generate {

    /*
     * Generate binary expression.
     */
    std::vector<X86Gp> expression(X86Compiler &c, BinaryExpr* expr) {
        if (expr->left->type.isPrimitive() && expr->right->type.isPrimitive()) {
            return { primitiveBinaryExpr(c, expr) };
        } else if (expr->left->type.isTuple() && expr->op->symbol == OperatorSymbol::DOT &&
		   expr->right->type.isPrimitive() && expr->right->type.type.primitive == DPrimitiveKind::INTEGER) {
	    return tupleAccessExpr(c, expr);
	}
	
        // Not returning before this point should have resulted in a semantic error during analysis of the binary expr.
        assert(false);
    }

}

