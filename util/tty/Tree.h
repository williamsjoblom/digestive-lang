#pragma once

#include <string>
#include <vector>

namespace TTY {
    
    struct TreeNode {
	/**
	 * Label.
	 */
	std::string label;
	
	/**
	 * Children.
	 */
	std::vector<TreeNode*> children;

	
	/**
	 * Constructor.
	 */
	TreeNode() { }
	
	/**
	 * Constructor.
	 */
	TreeNode(std::string label);
	
	/**
	 * Destructor.
	 */
	~TreeNode();	
    };

    /**
     * Dump tree.
     */
    void dump(TreeNode* root);
}
