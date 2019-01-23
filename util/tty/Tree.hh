#pragma once

#include <string>
#include <vector>

namespace TTY {

    /**
     * Tree node.
     */
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
	 * Label to be dumped.
	 */
	virtual std::string ttyLabel() = 0;


	/**
	 * Destructor.
	 */
	virtual ~TreeNode();	
    };

    /**
     * Dump tree.
     */
    void dump(TreeNode* root);
}
