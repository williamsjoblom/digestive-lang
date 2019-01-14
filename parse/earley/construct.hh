#pragma once

#include <map>
#include <string>
#include <stack>

#include "ast/Node.hh"
#include "ast/Expr.hh"
#include "ast/IntegerLiteral.hh"
#include "lexer/TokenQueue.hh"
#include "parse/earley/Chart.hh"


/**
 * Construct function type.
 */
typedef void (*constructFun)(std::stack<Node*>& stack,
			     const TokenQueue& tokens, const EState* state);


extern std::map<std::string, constructFun> constructTable;

/**
 * Initialize construct table with the core language
 * construct functions.
 */
void initializeConstructTable();

/**
 * Process node.
 */
void processNode(std::string label, std::stack<Node*>& stack,
		 const TokenQueue& tokens, const EState* state);


