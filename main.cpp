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

#include "ir/TACProgram.h"
#include "irgen/Program.h"
#include "util/File.h"

bool verbose = false;

/**
 * Main.
 */
#ifndef TEST
int main(int argc, char* argv[]) {    
    std::string path = "/home/wax/test.dg";
    
    for (int i = 1; i < argc; i++) {
	std::string arg = argv[i];
	if (arg == "-v") verbose = true;
	else if (i == argc - 1) path = arg;
    }
    
    // -----------------------

    TACProgram program;

    std::string source = readSourceFile(path);
    
    Lexer lexer;
    TokenQueue tokens = lexer.lex(source);

    auto root = Parse::unit(tokens);
    
    Scope rootScope;
    root->analyze(&rootScope);
    
    Generate::unit(program, root);

    program.dump();

    return 0;

    
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
