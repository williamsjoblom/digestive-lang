#pragma once

#include "semantic/Scope.hh"
#include "Decl.hh"
#include "BlockStmt.hh"
#include "VariableDecl.hh"

/**
 * Function declaration.
 */
class FunctionDecl : public Decl {
public:
    /**
     * Parameters.
     */
    std::vector<VariableDecl*>* parameters;
    
    /**
     * Function body.
     */
    BlockStmt* body;
    
    /**
     * Return type.
     */
    DType returnType;

    /**
     * Print IR after IR generation.
     */
    bool dumpIr;
    
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
                 DType returnType, bool dumpAssembly=false, bool dumpIr=false);
    /**
     * Destructor.
     */
    ~FunctionDecl();

    /**
     * Generate.
     */
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
