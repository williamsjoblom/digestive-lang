#include "ASTNode.hh"

#include <sstream>


ASTNode::ASTNode(std::string label) {
    this->label = label;
}


ASTNode::~ASTNode() {
    for (ASTNode* child : children)
	delete child;
}


void ASTNode::addChild(ASTNode* child) {
    children.push_front(child);
}


void ASTNode::addSymbol(std::string symbol) {
    symbols.push_front(symbol);
}


std::string ASTNode::toS(std::string indent) {
    std::stringstream ss;

    ss << indent << label;
    if (!symbols.empty()) {
	ss << " [ ";
	
	for (std::string symbol : symbols) {
	    ss << symbol << " ";
	}
	
	ss << "]";
    }
	
    if (!children.empty())
	ss << ":";
    ss << std::endl;
    
    for (ASTNode* child : children)
	ss << child->toS(indent + "  ");

    return ss.str();
}
