//
// Created by wax on 12/15/16.
//

#include "util/PrettyPrint.h"
#include "BinaryExpr.h"

BinaryExpr::BinaryExpr(Expr* left, Operator* op, Expr* right) {
    this->left = left;
    this->op = op;
    this->right = right;
}

void BinaryExpr::dump(size_t indent) {
    printIndent(indent);
    std::cout << "(" << std::endl;

    left->dump(indent + 1);
    op->dump(indent + 1);
    right->dump(indent + 1);

    printIndent(indent);
    std::cout << ")" << std::endl;
}
