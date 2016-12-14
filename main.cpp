#include <iostream>

#include "lexer/Lexer.h"
#include "ast/Stmt.h"
#include "parse/Parser.h"

int main() {
    std::string source = "func lol { }";

    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);
    tokens.dump();

    try {
        Stmt* root = Parse::file(tokens);
        root->dump();
    } catch (int i) {
        std::cout << "Syntax error " << i << std::endl;
    }

    return 0;
}