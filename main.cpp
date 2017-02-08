
#include <iostream>
#include <asmjit/asmjit.h>
#include <unistd.h>
#include <jit/JitContext.h>


#include "config.h"

#include "gen/Gen.h"
#include "lexer/Lexer.h"
#include "parse/Parse.h"
#include "interactive/Interactive.h"
#include "jit/Jit.h"

int main() {
    Jit jit;
    Interactive::start(&jit);

    jit.load("/home/wax/test1.dg");

    JitContext::dumpHandles();

    int first = jit.run();

    //usleep(100 * 1000);

    //jit.reload("/home/wax/test2.dg");
    //int second = jit.run();

    std::cout << "First run: " << first << std::endl;

    return 0;
/*
    //

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
    */
}