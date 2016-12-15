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
#include <ast/VariableExpr.h>
#include <ast/FunctionCall.h>


std::vector<Token> shuntingYard(TokenQueue& tokens);
Expr* rpnToExpr(TokenQueue& tokens);

namespace Parse {

    Expr* expression(TokenQueue& tokens) {
        std::vector<Token> rpn = shuntingYard(tokens);
        TokenQueue rpnQueue(&rpn);
        Expr* expr = rpnToExpr(rpnQueue);
        return expr;
    }

    std::vector<Expr*>* argumentList(TokenQueue& tokens) {
        std::vector<Expr*>* arguments = new std::vector<Expr*>();
        if (!tokens.eat(LPAR)) return nullptr;

        if (tokens.top().type != RPAR) {
            bool hasArg = true;
            while (hasArg) {
                arguments->push_back(Parse::expression(tokens));
                hasArg = tokens.eat(COMMA);
            }
        }

        tokens.expect(RPAR);

        return arguments;
    }

}

void mismatchedParenthesis(Token token) {
    parseError(token, "Mismatched parenthesis in expression");
}

std::vector<Token> shuntingYard(TokenQueue& tokens) {
    std::vector<Token> output;
    std::stack<Token> stack;

    int parDepth = 0;

    while (!tokens.empty()) {
        Token token = tokens.top();

        if(token.type == SEMICOL) break;
        if(token.type == COMMA) break;
        if(token.type == RPAR && parDepth == 0) break;

        tokens.pop();

        if (token.type == NUMBER) {
            output.push_back(token);
        } else if(token.type == IDENTIFIER) {
            if(tokens.top().type == LPAR) { // Function call
                output.push_back(token);

                int oldParDepth = parDepth;
                do {
                    if (tokens.top().type == LPAR) parDepth++;
                    if (tokens.top().type == RPAR) parDepth--;
                    output.push_back(tokens.pop());
                } while (parDepth > oldParDepth);

            } else { // Variable
                output.push_back(token);
            }
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
            parDepth++;
            stack.push(token);
        } else if (token.type == RPAR ) {
            parDepth--;
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

Expr* rpnToExpr(TokenQueue& tokens) {
    std::stack<Expr*> stack;

    Token first = tokens.top();

    while(!tokens.empty()) {
        Token token = tokens.pop();

        if (token.type == NUMBER) {
            int number = std::stoi(token.value);
            stack.push(new LiteralExpr(number));
        } else if (token.type == IDENTIFIER) {
            std::vector<Expr*>* argList = Parse::argumentList(tokens);
            if (argList != nullptr) {
                stack.push(new FunctionCall(token.value, argList));
            } else {
                stack.push(new VariableExpr(token.value));
            }
        } else if (Operator::isOperator(token.value)) {
            if (stack.size() < 2)
                parseError(token, "Missing operand in expression");

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
        parseError(first, "Missing operand in expression");
    if (stack.size() > 1)
        parseError(first, "Missing operator in expression");

    return stack.top();
}
