#include <iostream>
#include <ctime>

/**
 * Run unit tests.
 */
//#define TEST

#include "globals.h"
#include "util/BuildTimestamp.h"
#include "parse/Parse.h"
#include "lexer/Lexer.h"
#include "interactive/Interactive.h"

#include "ir/TACProgram.h"
#include "genir/Program.h"
#include "util/File.h"
#include "genasm/TACCompiler.h"
#include "parse/earley/BNFParser.h"
#include "parse/earley/BNF.h"
#include "parse/earley/Expr.h"

#include <asmjit/asmjit.h>

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

    if (verbose) {
	std::cout << " digestive " << version
		  << ", " << buildTimestamp() << std::endl;
    }

    std::string root = rootPath();
    if (root.empty()) {
	std::cerr << "'DGROOT' environment variable not set, exiting!" << std::endl;
	return 1;
    }

    std::string grammarPath = rootPath() + "/lang/dg.bnf";
    std::string grammar = readSourceFile(grammarPath);
    Lexer gl(grammar);
    TokenQueue gt = gl.readAll();
    BNFGrammar g = Earley::parseGrammar(gt);

    std::string source = readSourceFile(path);
    Lexer sl(source);
    TokenQueue st = sl.readAll();
    bool accepted = Earley::parse(g, "unit", st);

    std::cout << std::endl;
    if (accepted) std::cout << "String recognized by grammar";
    else std::cout << "String not recognized by grammar";
    std::cout << std::endl;
    
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
