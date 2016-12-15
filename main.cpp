#include <iostream>

#include "util/PrettyPrint.h"
#include "lexer/Lexer.h"
#include "ast/Stmt.h"
#include "parse/Parse.h"

int main() {
    std::string source = "var x = 1 + f(3 + 3, 44, x(11, 22), 11);";

    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);

    /*
    std::cout << "Source -> Tokens =>" << std::endl;
    printIndent(1); tokens.dump();
    */

    std::cout << std::endl << "Tokens -> AST =>" << std::endl;
    try {
        Stmt* root = Parse::file(tokens);
        root->dump(1);
    } catch (int i) {
        std::cout << "Syntax error " << i << std::endl;
    }

    return 0;
}