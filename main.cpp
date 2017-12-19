#include <iostream>
#include <ctime>

/**
 * Run unit tests.
 */
//#define TEST

#include "globals.h"
#include "parse/Parse.h"
#include "lexer/Lexer.h"
#include "interactive/Interactive.h"

#include "ast/Stmt.h"
#include "ir/TACEnv.h"

bool verbose = false;

/**
 * Main.
 */
#ifndef TEST
int main(int argc, char* argv[]) {
    std::string source = argv[1];

    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);

    Stmt* e = Parse::statement(tokens);
    Scope s;
    e->analyze(&s);

    TACEnv env;
    e->generate(env);
    
    env.dump();

    return 0;
    // TACEnv env;

    // TACType t0 = TACType(TACKind::UNSIGNED, 4);
    // auto v0 = env.newVar(t0);

    // TACType t1 = TACType(TACKind::UNSIGNED, 4);
    // auto v1 = env.newVar(t0);

    // TACType t2 = TACType(TACKind::UNSIGNED, 4);
    // auto v2 = env.newVar(t0);
    
    // env.add(TACC::add, v0, v1, v2);
    // env.add(TACC::div, v0, v1, v2);
    
    // env.dump();
    
    // return 0;
    
    std::string path = "/home/wax/test.dg";
    
    for (int i = 1; i < argc; i++) {
	std::string arg = argv[i];
	if (arg == "-v") verbose = true;
	else if (i == argc - 1) path = arg;
    }
    
    Jit jit;
    Interactive::start(&jit);

    std::clock_t compile_t0 = std::clock();

    bool loaded = jit.load(path);
    
    
    std::clock_t compile_t1 = std::clock();
    double compile_dt = double(compile_t1 - compile_t0) / (CLOCKS_PER_SEC / 1000);
    
    if (verbose)
	std::cout << "Compilation in " << compile_dt << " ms" << std::endl;
    
    if(loaded) {
	if (verbose)
	    std::cout << "Run " << path << " => " << std::endl;

	std::clock_t run_t0 = std::clock();
	
	int result = jit.run();
	
	std::clock_t run_t1 = std::clock();
	double run_dt = double(run_t1 - run_t0) / (CLOCKS_PER_SEC / 1000);
	if (verbose)
	    std::cout << "Completed in " << run_dt << " ms" << std::endl;
	
        return result;
    }
    
    return -1;
}
#endif


