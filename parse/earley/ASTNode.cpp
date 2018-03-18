#include "ASTNode.h"

#include <sstream>


ASTNode::ASTNode(std::string label) {
    this->label = label;
}


ASTNode::~ASTNode() {
    for (ASTNode* child : children)
	delete child;
}


void ASTNode::addChild(ASTNode* child) {
    children.push_back(child);
}


std::string ASTNode::toS(std::string indent) {
    std::stringstream ss;
    
    ss << indent << label << ":" << std::endl;
    for (ASTNode* child : children)
	ss << child->toS(indent + "  ");

    return ss.str();
}
