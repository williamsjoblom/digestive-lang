//
// Created by wax on 6/28/17.
//

#include "Parse.h"
#include "ParseError.h"

#include <vector>
#include <stack>
#include <ast/Operator.h>
#include <ast/Literal.h>
#include <ast/BinaryExpr.h>
#include <assert.h>
#include <ast/VariableExpr.h>
#include <ast/FunctionCall.h>
#include <ast/UnaryExpr.h>
#include <ast/IntegerLiteral.h>
#include <ast/TupleExpr.h>


std::vector<Token> shuntingYard(TokenQueue& tokens);
Expr* rpnToExpr(TokenQueue& tokens);
Expr* buildUnary(std::stack<Operator*>& operators, Expr* expr);

namespace Parse {

    Expr* expression(TokenQueue& tokens) {
        std::vector<Token> rpn = shuntingYard(tokens);
        TokenQueue rpnQueue(rpn);
	Expr* expr = rpnToExpr(rpnQueue);
        return expr;
    }

    
    std::vector<Expr*>* argumentList(TokenQueue& tokens) {
        if (!tokens.eat(LARGPAR)) return nullptr;
        std::vector<Expr*>* arguments = new std::vector<Expr*>();

        if (tokens.top().type != RARGPAR) {
            bool hasArg = true;
            while (hasArg) {
                arguments->push_back(Parse::expression(tokens));
                hasArg = tokens.eat(COMMA);
            }
        }

        tokens.expect(RARGPAR);

        return arguments;
    }
}


/**
 * Throw a mismatched paranthesis error.
 */
void mismatchedParenthesis(Token token) {
    parseError(token, "Mismatched parenthesis in expression");
}


/**
 * Transform infix expression to postfix.
 *
 * A heavily modified implementation of the shunting yard algorithm.
 * Modified to support unary operators, variable argument function calls and tuples.
 * Currently a mess (although a functional mess) and will have to be reworked at 
 * some point, preferably using more of a data-oriented approach.
 */
std::vector<Token> shuntingYard(TokenQueue& tokens) {
    std::vector<Token> output;
    std::stack<Token> stack;

    bool expectUnary = true;

    int parDepth = 0;

    while (!tokens.empty()) {
        Token token = tokens.top();

        if(token.type == SEMICOL) break;
        if(token.type == COMMA && parDepth == 0) break;
        if(token.type == RPAR && parDepth == 0) break;	
	if(token.type == RARGPAR && parDepth == 0) break;
	
        tokens.pop();

        if (token.type == NUMBER) {
            output.push_back(token);
            expectUnary = false;
        } else if(token.type == IDENTIFIER) {
	    if(tokens.top().type == LPAR) { // Function call
		output.push_back(token);
		
		Token leftArgPar(tokens.pop());
		leftArgPar.type = TokenType::LARGPAR;
		output.push_back(leftArgPar);
		
		int oldParDepth = parDepth - 1;
                do {
                    if (tokens.top().type == LPAR) parDepth++;
                    if (tokens.top().type == RPAR) parDepth--;
                    output.push_back(tokens.pop());
                } while (parDepth > oldParDepth);
		parDepth = oldParDepth;

		// Replace the ending parenthesis with an arglist paranthesis.
		Token rightArgPar(output.back());
		rightArgPar.type = TokenType::RARGPAR;
		output.pop_back();
		output.push_back(rightArgPar);
	    } else { // Variable
                output.push_back(token);
            }

            expectUnary = false;
        } else if (Operator::isOperator(token.value)) {
            Operator o1(token.value);

            if (expectUnary) {
                output.push_back(token);
                continue;
            }
	    
            while (!stack.empty() && Operator::isOperator(stack.top().value)) {
                Token top = stack.top();
                Operator o2(top.value);
		
		if ((o1.isLeftAssociative() && o1.precedence > o2.precedence) ||
		    (o1.isRightAssociative() && o1.precedence <= o2.precedence)) {
                    break;
		}

		output.push_back(top);
		stack.pop();
            }

            stack.push(token);

            expectUnary = true;
        } else if (token.type == LPAR) {
            parDepth++;

            stack.push(token);
            output.push_back(token);
        } else if (token.type == RPAR ) {
            parDepth--;

            Token top;
            while (stack.top().type != LPAR && stack.top().type != COMMA) {
                if (stack.size() <= 0) // 1
                    mismatchedParenthesis(top);

                top = stack.top();
                output.push_back(top);
                stack.pop();
            }

            output.push_back(token);
            stack.pop(); // Pop LPAR

            expectUnary = false;

        } else if (token.type == COMMA) {
            Token top;
            while (stack.top().type != LPAR && stack.top().type != COMMA) {
                if (stack.size() <= 0) // 1
                    mismatchedParenthesis(top);

                top = stack.top();
                output.push_back(top);
                stack.pop();
            }

            output.push_back(token);
            stack.pop();
            stack.push(token);

        }
    }

    while(!stack.empty()) {
        Token top = stack.top();

        if (top.type == LPAR || top.type == RPAR)
            mismatchedParenthesis(top);

        output.push_back(top);
        stack.pop();
    }

    return output;
}

Expr* rpnToExpr(TokenQueue& tokens) {
    std::stack<Expr*> stack;

    Token first = tokens.top();

    std::stack<Operator*> unaryOperators;

    if (!tokens.empty() && Operator::isOperator(tokens.top().value)) {
        Token t = tokens.top();
        Operator* op = new Operator(t.value);
        if (op->isUnary()) {
            tokens.pop();
            unaryOperators.push(op);
        } else {
            parseError(t, "Operator not unary");
        }

    }

    int tupleSize = 1;

    std::stack<int> tupleSizeStack = std::stack<int>();
    while(!tokens.empty()) {
        Token token = tokens.pop();

        if (token.type == NUMBER) {
            int number = std::stoi(token.value);
            Expr *expr = buildUnary(unaryOperators, new IntegerLiteral(number));
            stack.push(expr);
        } else if (token.type == IDENTIFIER) {
            std::vector<Expr*>* argList = Parse::argumentList(tokens);

            Expr* expr;
            if (argList != nullptr) {
                expr = buildUnary(unaryOperators, new FunctionCall(token.value, argList));
            } else {
                expr = buildUnary(unaryOperators, new VariableExpr(token.value));
            }

            stack.push(expr);

        } else if (Operator::isOperator(token.value)) {
            if (stack.size() == 0) {
                parseError(token, "Missing operand in expression");
            }

            if (stack.size() == 1) {
                unaryOperators.push(new Operator(token.value));
            } else {
                Expr *right = stack.top();
                stack.pop();
                Expr *left = stack.top();
                stack.pop();
                Operator *op = new Operator(token.value);
                stack.push(new BinaryExpr(left, op, right));
            }
        } else if (token.type == LPAR) {
            tupleSizeStack.push(tupleSize);
            tupleSize = 1;
        } else if(token.type == RPAR) {
            if(tupleSize != 1) {
                std::vector<Expr*> tupleValues = std::vector<Expr*>(tupleSize);
                for (int i = 0; i < tupleSize; i++) {
                    Expr* v = stack.top();
                    tupleValues[(tupleSize - 1) - i] = v;
                    stack.pop();
                }

                stack.push(new TupleExpr(tupleValues));
            }

            tupleSize = tupleSizeStack.top();
            tupleSizeStack.pop();

        } else if (token.type == COMMA) {
            tupleSize++;
        } else {
            assert(false);
        }
    }

    // TODO: free memory if expression turns out invalid.
    if (stack.empty())
        parseError(first, "Missing operand in expression");
    if (stack.size() > 1)
        parseError(first, "Missing operator in expression");

    return stack.top();
}

Expr* buildUnary(std::stack<Operator*>& operators, Expr* expr) {
    if (operators.empty()) return expr;

    Operator* op = operators.top();
    operators.pop();

    return new UnaryExpr(op, buildUnary(operators, expr));
}
