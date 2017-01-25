//
// Created by wax on 1/24/17.
//

#ifndef DIG_UNIT_H
#define DIG_UNIT_H


#include "FunctionDecl.h"

class Unit : public Node {
public:
    std::vector<Stmt*> statements;
    std::vector<FunctionDecl*> functions;

    Unit(std::vector<Stmt*> statements, std::vector<FunctionDecl*> functions);

    virtual void generate(X86Compiler &c);

    void analyze(Scope* scope);
    void dump(size_t indent);

private:
};


#endif //DIG_UNIT_H
