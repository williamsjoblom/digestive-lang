//
// Created by wax on 12/15/16.
//

#include "Parse.h"
#include "ParseError.h"

#include <vector>
#include <stack>
#include <ast/Operator.h>
#include <ast/LiteralExpr.h>
#include <ast/BinaryExpr.h>
#include <assert.h>


std::vector<Token> shuntingYard(TokenQueue& tokens);
Expr* rpnToExpr(std::vector<Token> tokens);

namespace Parse {

    Expr* expression(TokenQueue& tokens) {
        std::vector<Token> rpn = shuntingYard(tokens);
        Expr* expr = rpnToExpr(rpn);
        return expr;
    }

}

void mismatchedParenthesis(Token token) {
    parseError(token, "Mismatched parenthesis in expression");
}

std::vector<Token> shuntingYard(TokenQueue& tokens) {
    std::vector<Token> output;
    std::stack<Token> stack;

    while (!tokens.empty()) {
        Token token = tokens.pop();

        if(token.type == SEMICOL) break;

        if (token.type == NUMBER) {
            output.push_back(token);
        } else if (Operator::isOperator(token.value)) {
            Operator o1(token.value);

            while (!stack.empty() && Operator::isOperator(stack.top().value)) {
                Token top = stack.top();
                Operator o2(top.value);

                if(o1.precedence <= o2.precedence) {
                    output.push_back(top);
                    stack.pop();
                } else {
                    break;
                }
            }

            stack.push(token);
        } else if (token.type == LPAR) {
            stack.push(token);
        } else if (token.type == RPAR ) {
            Token top;
            while (stack.top().type != LPAR) {
                if (stack.size() <= 1)
                    mismatchedParenthesis(top);

                top = stack.top();
                output.push_back(top);
                stack.pop();
            }
            stack.pop(); // Pop LPAR
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

Expr* rpnToExpr(std::vector<Token> tokens) {
    std::stack<Expr*> stack;

    for (Token token : tokens) {
        if (token.type == NUMBER) {
            int number = std::stoi(token.value);
            stack.push(new LiteralExpr(number));
        } else if(Operator::isOperator(token.value)) {
            Expr* right = stack.top(); stack.pop();
            Expr* left = stack.top(); stack.pop();
            Operator* op = new Operator(token.value);
            stack.push(new BinaryExpr(left, op, right));
        } else {
            assert(false);
        }
    }

    // TODO: free memory if expression turns out invalid.
    if (stack.empty())
        parseError(tokens.back(), "Missing operand in expression");
    if (stack.size() > 1)
        parseError(tokens.back(), "Missing operator in expression");

    return stack.top();
}
