//
// Created by wax on 12/14/16.
//

#ifndef DIG_FUNCTIONDECL_H
#define DIG_FUNCTIONDECL_H

#include "semantic/Scope.h"
#include "Decl.h"
#include "BlockStmt.h"
#include "VariableDecl.h"

class FunctionDecl : public Decl {
public:
    std::vector<VariableDecl*>* parameters;
    BlockStmt* body;
    DType returnType;


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
     * IR id.
     */
    int irId;

    /**
     * Byte size of code.
     */
    size_t codeSize;

    /**
     * Constructor.
     */
    FunctionDecl(std::string identifier, std::vector<VariableDecl*>* parameters, BlockStmt* body,
                 DType returnType, bool dumpAssembly=false);
    /**
     * Destructor.
     */
    ~FunctionDecl();

    /**
     * Generate.
     */
    void generate(X86Compiler& c) override;
    void generate(TACFun* env) override;

    /**
     * Semantically analyze node.
     */
    void analyze(Scope* scope) override;

    virtual bool equals(const Node& other) const override;
    virtual void dump(size_t indent) override;

    bool matchesSignature(const FunctionDecl& other) const;
    FuncSignatureX bCreatePrototype();

    int stackSize() override;
};


#endif //DIG_FUNCTIONDECL_H
