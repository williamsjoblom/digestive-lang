#pragma once

#include <string>
#include <list>

#include "BNF.h"


/**
 * AST node.
 */
class ASTNode {
public:
    /**
     * Label.
     */
    std::string label;
    
    /**
     * Children.
     */
    std::list<ASTNode*> children;


    /**
     * Symbols.
     */
    std::list<std::string> symbols;

    
    /**
     * Constructor.
     */
    ASTNode(std::string label);
    
    
    /**
     * Destructor.
     */
    ~ASTNode();

    
    /**
     * Add node.
     */
    void addChild(ASTNode* child);

    
    /**
     * Add symbol.
     */
    void addSymbol(std::string symbol);


    /**
     * To string.
     */
    std::string toS(std::string indent="");
};
