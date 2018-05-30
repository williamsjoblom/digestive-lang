#pragma once

#include <string>
#include <list>

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
     * To string.
     */
    std::string toS(std::string indent="");
};
