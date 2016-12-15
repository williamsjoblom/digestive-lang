#include <iostream>

#include "util/PrettyPrint.h"
#include "lexer/Lexer.h"
#include "ast/Stmt.h"
#include "parse/Parse.h"

int main() {
    std::string source = "func hello { 1 + 2 * 3; }";

    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);

    std::cout << "Source -> Tokens =>" << std::endl;
    printIndent(1); tokens.dump();

    std::cout << std::endl << "Tokens -> AST =>" << std::endl;
    try {
        Stmt* root = Parse::file(tokens);
        root->dump(1);
    } catch (int i) {
        std::cout << "Syntax error " << i << std::endl;
    }

    return 0;
}