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
    const Type* returnType;


    /**
     * Print assembly after code generation.
     */
    bool dumpAssembly;

    /**
     * Index of function pointer in current JitContext.
     */
    int bHandleIndex;

    /**
     * Function prototype.
     */
    FuncSignatureX* baPrototype;

    /**
     * Byte size of code.
     */
    size_t codeSize;

    /**
     * Constructor.
     */
    FunctionDecl(std::string identifier, std::vector<VariableDecl*>* parameters, BlockStmt* body,
                 const Type* returnType, bool dumpAssembly=false);
    /**
     * Destructor.
     */
    ~FunctionDecl();

    /**
     * Generate.
     */
    void generate(X86Compiler &c);

    /**
     * Semantically analyze node.
     */
    void analyze(Scope* scope);

    virtual bool equals(const Node& other) const;
    virtual void dump(size_t indent) override;

    bool matchesSignature(const FunctionDecl& other) const;
    FuncSignatureX bCreatePrototype();

    int stackSize();
};


#endif //DIG_FUNCTIONDECL_H
