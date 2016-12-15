//
// Created by wax on 12/15/16.
//

#include "LiteralExpr.h"
#include "util/PrettyPrint.h"

LiteralExpr::LiteralExpr(int value) {
    this->value = value;
}

void LiteralExpr::dump(size_t indent) {
    printIndent(indent);
    std::cout << value  << std::endl;
}


