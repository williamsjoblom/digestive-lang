#include <iostream>

#include "util/PrettyPrint.h"
#include "lexer/Lexer.h"
#include "ast/Stmt.h"
#include "parse/Parse.h"

int main() {
    std::string source = "var x = 11; pln x;";

    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);


    std::cout << "Source -> Tokens =>" << std::endl;
    tokens.dump();

    try {
        Stmt* root = Parse::file(tokens);
        std::cout << std::endl << "Tokens -> AST =>" << std::endl;
        root->dump();

        Scope* fileScope = new Scope();
        root->analyze(fileScope);
        std::cout << std::endl << "AST -> Scope =>" << std::endl;
        fileScope->dump();

    } catch (int i) {
        std::cout << "error " << i << std::endl;
    }

    return 0;
}