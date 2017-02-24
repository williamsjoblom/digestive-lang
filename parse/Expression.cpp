//
// Created by wax on 12/15/16.
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
#include "ast/RealLiteral.h"


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
        if (!tokens.eat(LPAR)) return nullptr;
        std::vector<Expr*>* arguments = new std::vector<Expr*>();

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

    bool expectUnary = true;

    int parDepth = 0;

    while (!tokens.empty()) {
        Token token = tokens.top();

        if(token.type == SEMICOL) break;
        if(token.type == COMMA) break;
        if(token.type == RPAR && parDepth == 0) break;

        tokens.pop();

        if (token.type == INTEGER || token.type == REAL) {
            output.push_back(token);
            expectUnary = false;
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

                if(o1.precedence <= o2.precedence) {
                    output.push_back(top);
                    stack.pop();
                } else {
                    break;
                }
            }

            stack.push(token);

            expectUnary = true;
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

    while(!tokens.empty()) {
        Token token = tokens.pop();

        if (token.type == INTEGER) {
            int number = std::stoi(token.value);
            Expr *expr = buildUnary(unaryOperators, new IntegerLiteral(number));
            stack.push(expr);
        } else if (token.type == REAL) {
            double number = std::stod(token.value);
            Expr *expr = buildUnary(unaryOperators, new RealLiteral(number));
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
                Expr* right = stack.top(); stack.pop();
                Expr* left = stack.top(); stack.pop();
                Operator* op = new Operator(token.value);
                stack.push(new BinaryExpr(left, op, right));
            }
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
