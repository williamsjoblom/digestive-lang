//
// Created by wax on 12/15/16.
//

#include "gen/Gen.h"
#include "semantic/TypeUtil.h"
#include "util/PrettyPrint.h"
#include "BinaryExpr.h"
#include "irgen/BinaryExpr.h"

BinaryExpr::BinaryExpr(Expr* left, Operator* op, Expr* right) {
    this->left = left;
    this->op = op;
    this->right = right;
}

BinaryExpr::~BinaryExpr() {
    delete left;
    delete op;
    delete right;
}

void BinaryExpr::analyze(Scope *scope) {
    left->analyze(scope);
    if (op->symbol != OperatorSymbol::DOT)
	right->analyze(scope);
    
    type = resultingType(left, *op, right);
}

void BinaryExpr::dump(size_t indent) {
    std::cout << "(";

    left->dump(indent + 1);
    std::cout << " ";
    op->dump(indent + 1);
    std::cout << " ";
    right->dump(indent + 1);

    std::cout << ")";
}

TACOp BinaryExpr::generate(TACFun* env) {
    return Generate::binaryExpr(env, this);
}

Regs BinaryExpr::generate(X86Compiler &c) {
    return Generate::expression(c, this);
}

bool BinaryExpr::equals(const Node &other) const {
    const BinaryExpr* o = dynamic_cast<const BinaryExpr*>(&other);
    if (o == nullptr) return false;
    return *o->left == *left && *o->op == *op && *o->right == *right;
}


