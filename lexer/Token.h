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
    STRING,
    
    // Symbols
    PLUS,
    MINUS,
    MUL,
    DIV,

    ASSIGN,

    COL,
    SEMICOL,
    COMMA,
    LBRACK,
    RBRACK,
    LPAR,
    RPAR,
    // Argument list parantheses.
    // Replaces regular paranthesis during infix to postfix parsing.
    LARGPAR,
    RARGPAR,
    
    PIPE,

    EQ,
    NOTEQ,
    LESSEQ,
    GREATEREQ,
    LESS,
    GREATER,

    RIGHTARROW,
    DOT,

    AT,

    UNKNOWN,
    END_OF_FILE,
};


/**
 * Token type string representations.
 */
static std::string tokenTypeToS[] = {
    "IDENTIFIER",
    "NUMBER",
    "STRING",
    "PLUS",
    "MINUS",
    "MUL",
    "DIV",

    "ASSIGN",

    "COL",
    "SEMICOL",
    "COMMA",
    "LBRACK",
    "RBRACK",
    "LPAR",
    "RPAR",
    
    "LARGPAR",
    "RARGPAR",

    "PIPE",

    "EQ",
    "NOTEQ",
    "LESSEQ",
    "GREATEREQ",
    "LESS",
    "GREATER",

    "RIGHTARROW",
    "DOT",

    "AT",
    
    "UNKNOWN",
    "EOF",
};


struct Token {
    TokenType type;
    std::string value;
    int row, col;
    int index;

    void dump() { std::cout << "[" << row << ":" << col << "]" << value << std::endl; }
};


static std::map<std::string, TokenType> valueToTokenType =
{
    {"+", PLUS},
    {"-", MINUS},
    {"*", MUL},
    {"/", DIV},

    {"=", ASSIGN},

    {":", COL},
    {";", SEMICOL},
    {",", COMMA},
    {"{", LBRACK},
    {"}", RBRACK},
    {"(", LPAR},
    {")", RPAR},

    {"|", PIPE},
    
    {"==", EQ},
    {"!=", NOTEQ},
    {"<=", LESSEQ},
    {">=", GREATEREQ},
    {"<", LESS},
    {">", GREATER},

    {"->", RIGHTARROW},
    {".", DOT},

    {"@", AT},
};



#endif //DIG_TOKEN_H
