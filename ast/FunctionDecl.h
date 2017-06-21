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

    int bHandleIndex;
    FuncSignatureX* baPrototype;
    size_t codeSize;

    FunctionDecl(std::string identifier, std::vector<VariableDecl*>* parameters, BlockStmt* body);
    ~FunctionDecl();

    void generate(X86Compiler &c);
    void analyze(Scope* scope);
    virtual bool equals(const Node& other) const;
    virtual void dump(size_t indent);

    bool matchesSignature(const FunctionDecl& other) const;
    FuncSignatureX bGetFuncPrototype();

    int stackSize() override;
};


#endif //DIG_FUNCTIONDECL_H
