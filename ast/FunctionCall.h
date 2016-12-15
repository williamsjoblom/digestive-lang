//
// Created by wax on 12/15/16.
//

#ifndef DIG_FUNCTIONCALL_H
#define DIG_FUNCTIONCALL_H


#include <vector>
#include "Expr.h"

class FunctionCall : public Expr {
public:
    std::string identifier;
    std::vector<Expr*>* arguments;

    FunctionCall(std::string identifier, std::vector<Expr*>* arguments);

    void dump(size_t indent);
};


#endif //DIG_FUNCTIONCALL_H
