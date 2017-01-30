//
// Created by wax on 12/14/16.
//

#ifndef DIG_FUNCTIONDECL_H
#define DIG_FUNCTIONDECL_H

#include <semantic/Scope.h>
#include "Decl.h"
#include "BlockStmt.h"
#include "VariableDecl.h"

class FunctionDecl : public Decl {
public:
    std::vector<VariableDecl*>* parameters;
    BlockStmt* body;

    Label bEntryLabel;
    int bHandleIndex;

    FunctionDecl(std::string identifier, std::vector<VariableDecl*>* parameters, BlockStmt* body);

    void generate(X86Compiler &c);
    void analyze(Scope* scope);
    virtual bool equals(const Node& other) const;
    virtual void dump(size_t indent);
};


#endif //DIG_FUNCTIONDECL_H
