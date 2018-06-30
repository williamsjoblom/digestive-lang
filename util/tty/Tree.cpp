#include "Tree.h"

#include <iostream>

#include "Color.h"


namespace TTY {
    
    TreeNode::TreeNode(std::string label) {
	this->label = label;
    }

    
    TreeNode::~TreeNode() {
	for (TreeNode* child : children)
	    delete child;
    }
    

    void dump(TreeNode* node, std::string indent) {
	std::vector<TreeNode*>& children = node->children;
	for (auto it = children.begin(); it != children.end(); ++it) {
	    TreeNode* child = *it;

	    std::string childIndent;
	    if (child == children.back()) {
		std::cout << indent << c("└─", WHITE);
		childIndent = indent + "  ";
	    } else {
		std::cout << indent << c("├─", WHITE);
		childIndent = indent + c("│ ", WHITE);
	    }
	    
	    std::cout << child->label
		      << std::endl;

	    dump(child, childIndent);
	}
    }

    
    void dump(TreeNode* root) {
	std::cout << root->label << std::endl;
	dump(root, "");
    }
    
};
