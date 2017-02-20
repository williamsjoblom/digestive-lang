//
// Created by wax on 12/13/16.
//

#ifndef DIG_TOKEN_H
#define DIG_TOKEN_H

#include <iostream>
#include <map>

enum TokenType {
    IDENTIFIER,
    NUMBER,

    // Symbols
    PLUS,
    MINUS,
    MUL,
    DIV,

    ASSIGN,

    SEMICOL,
    COMMA,
    LBRACK,
    RBRACK,
    LPAR,
    RPAR,

    EQ,
    NOTEQ,
    LESSEQ,
    GREATEREQ,
    LESS,
    GREATER,

    END_OF_FILE,
};

struct Token {
    TokenType type;
    std::string value;
    int row, col;

    void dump() { std::cout << "[" << row << ":" << col << "]" << value << std::endl; }
};

static std::map<std::string, TokenType> symbolToTokenType =
        {
                {"+", PLUS},
                {"-", MINUS},
                {"*", MUL},
                {"/", DIV},

                {"=", ASSIGN},

                {";", SEMICOL},
                {",", COMMA},
                {"{", LBRACK},
                {"}", RBRACK},
                {"(", LPAR},
                {")", RPAR},

                {"==", EQ},
                {"!=", NOTEQ},
                {"<=", LESSEQ},
                {">=", GREATEREQ},
                {"<", LESS},
                {">", GREATER},
        };

#endif //DIG_TOKEN_H
