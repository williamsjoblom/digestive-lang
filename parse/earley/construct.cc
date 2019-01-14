#include "construct.hh"

#include "ast/BinaryExpr.hh"
#include "ast/ExprStmt.hh"
#include "ast/Unit.hh"
#include "ast/PlnStmt.hh"

std::map<std::string, constructFun> constructTable;


/**
 * @IntLiteral
 */
void constructIntLiteral(std::stack<Node*>& stack,
		     const TokenQueue& tokens, const EState* state) {
    int pos = state->origin;
    int value = stoi(tokens.at(pos).value);

    stack.push(new IntegerLiteral(value));
}


/**
 * @IntLiteral
 */
void constructVarExpr(std::stack<Node*>& stack,
		     const TokenQueue& tokens, const EState* state) {
    int pos = state->origin;
    int value = stoi(tokens.at(pos).value);

    stack.push(new IntegerLiteral(value));
}


/**
 * @AddExpr
 */
void constructAddExpr(std::stack<Node*>& stack,
		  const TokenQueue& tokens, const EState* state) {
    Expr* rhs = dynamic_cast<Expr*>(stack.top()); stack.pop();
    Expr* lhs = dynamic_cast<Expr*>(stack.top()); stack.pop();
    assert(rhs);
    assert(lhs);

    Operator* op = new Operator(OperatorSymbol::PLUS);
    stack.push(new BinaryExpr(lhs, op, rhs));
}


/**
 * @SubExpr
 */
void constructSubExpr(std::stack<Node*>& stack,
		  const TokenQueue& tokens, const EState* state) {
    Expr* rhs = dynamic_cast<Expr*>(stack.top()); stack.pop();
    Expr* lhs = dynamic_cast<Expr*>(stack.top()); stack.pop();
    assert(rhs);
    assert(lhs);

    Operator* op = new Operator(OperatorSymbol::MINUS);
    stack.push(new BinaryExpr(lhs, op, rhs));
}


/**
 * @MulExpr
 */
void constructMulExpr(std::stack<Node*>& stack,
		      const TokenQueue& tokens, const EState* state) {
    Expr* rhs = dynamic_cast<Expr*>(stack.top()); stack.pop();
    Expr* lhs = dynamic_cast<Expr*>(stack.top()); stack.pop();
    assert(rhs);
    assert(lhs);

    Operator* op = new Operator(OperatorSymbol::MUL);
    stack.push(new BinaryExpr(lhs, op, rhs));
}


/**
 * @MulExpr
 */
void constructDivExpr(std::stack<Node*>& stack,
		      const TokenQueue& tokens, const EState* state) {
    Expr* rhs = dynamic_cast<Expr*>(stack.top()); stack.pop();
    Expr* lhs = dynamic_cast<Expr*>(stack.top()); stack.pop();
    assert(rhs);
    assert(lhs);

    Operator* op = new Operator(OperatorSymbol::DIV);
    stack.push(new BinaryExpr(lhs, op, rhs));
}


/**
 * @ExprStmt
 */
void constructExprStmt(std::stack<Node*>& stack,
		      const TokenQueue& tokens, const EState* state) {
    Expr* e = dynamic_cast<Expr*>(stack.top()); stack.pop();
    stack.push(new ExprStmt(e));
}


/**
 * @Block
 */
void constructBlock(std::stack<Node*>& stack,
		    const TokenQueue& tokens, const EState* state) {
    // Allocate vector with space for all contained statements.
    std::vector<Stmt*> statements(state->childCount);

    std::cout << "!!!block: " << state->childCount << std::endl;
    
    for (int i = state->childCount - 1; i >= 0; i--) {
	Stmt* stmt = dynamic_cast<Stmt*>(stack.top()); stack.pop();
	assert(stmt);
	statements[i] = stmt;
    }

    stack.push(new BlockStmt(statements));
}


/**
 * @CompilationUnit
 */
void constructUnit(std::stack<Node*>& stack,
		    const TokenQueue& tokens, const EState* state) {

    std::vector<Node*> nodes(stack.size());
    for (int i = state->childCount - 1; i >= 0; i--) {
	nodes[i] = stack.top(); stack.pop();
    }

    std::vector<Stmt*> statements;
    std::vector<FunctionDecl*> funs;

    for (Node* node : nodes) {
	if (auto stmt = dynamic_cast<Stmt*>(node))
	    statements.push_back(stmt);
	else if (auto funDecl = dynamic_cast<FunctionDecl*>(node))
	    funs.push_back(funDecl);
    }

    stack.push(new Unit(statements, funs));
}


/**
 * @Pln
 */
void constructPln(std::stack<Node*>& stack,
		  const TokenQueue& tokens, const EState* state) {
    Expr* expr = dynamic_cast<Expr*>(stack.top()); stack.pop();
    stack.push(new PlnStmt(expr));
}
    


void initializeConstructTable() {
    constructTable["IntLiteral"] = (constructFun) constructIntLiteral;
    constructTable["AddExpr"] = (constructFun) constructAddExpr;
    constructTable["SubExpr"] = (constructFun) constructSubExpr;
    constructTable["MulExpr"] = (constructFun) constructMulExpr;
    constructTable["DivExpr"] = (constructFun) constructDivExpr;
    constructTable["ExprStmt"] = (constructFun) constructExprStmt;
    constructTable["Block"] = (constructFun) constructBlock;
    constructTable["CompilationUnit"] = (constructFun) constructUnit;
    constructTable["Pln"] = (constructFun) constructPln;
}


/**
 * Process node.
 */
void processNode(std::string label, std::stack<Node*>& stack,
		 const TokenQueue& tokens, const EState* state) {
    constructFun construct = constructTable[label];

    if (construct) {
	construct(stack, tokens, state);
    } else {
	assert(false); // Error: undefined construct function.
    }
}
