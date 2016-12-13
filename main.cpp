#include <iostream>
#include "lexer/Lexer.h"

int main() {
    std::string source = "hello world 123 +-////00988qwe--";

    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);
    tokens.dump();

    return 0;
}