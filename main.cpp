
#include <iostream>
#include <asmjit/asmjit.h>
#include <interactive/Interactive.h>

#include "config.h"

#include "gen/Gen.h"
#include "lexer/Lexer.h"
#include "parse/Parse.h"

int main() {


    Interactive::start();

    std::string source = "func f() { return 3; } return f();";

    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);
#if DUMP_TOK
    std::cout << "Source -> Tokens =>" << std::endl;
    tokens.dump();
#endif

    try {
        Unit* root = Parse::unit(tokens);
#if DUMP_AST
        std::cout << std::endl << "Tokens -> AST =>" << std::endl;
        root->dump(0);
#endif

        Scope* fileScope = new Scope();
        root->analyze(fileScope);
#if DUMP_SYM
        std::cout << std::endl << "AST -> Scope =>" << std::endl;
        fileScope->dump();
#endif

        JitRuntime runtime;
        ProgramType program = Generate::program(&runtime, root);
        std::cout << std::endl << "Program exited with code " << program() << std::endl;
        runtime.release((void*) program);

    } catch (int i) {
        std::cout << "compilation error " << i << std::endl;
    }

    return 0;
}