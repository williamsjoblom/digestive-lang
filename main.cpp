#include <iostream>
#include <ctime>

#include <asmjit/asmjit.h>
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include "options.h"
#include "util/BuildTimestamp.h"
#include "parse/Parse.h"
#include "lexer/Lexer.h"
#include "interactive/Interactive.h"
#include "util/tty/Tree.h"

#include "ir/TACProgram.h"
#include "genir/Program.h"
#include "util/File.h"
#include "util/Path.h"
#include "genasm/TACCompiler.h"
#include "parse/earley/BNFParser.h"
#include "parse/earley/BNF.h"
#include "parse/earley/Parse.h"
#include "parse/earley/ASTNode.h"

/**
 * Globals defined as 'extern' in 'options.h'.
 */
bool verbose = false;
bool runTests = false;

/**
 * Forward declarations.
 */
std::string parseArgs(int argc, char* argv[]);
bool runTestSuite();

/**
 * Main.
 */
int main(int argc, char* argv[]) {    
    std::string pathArg = parseArgs(argc, argv);

    if (verbose) {
	std::cout << " digestive " << version
		  << ", " << buildTimestamp() << std::endl;
    }

    
    if (!hasRootDirPath()) {
	std::cerr << "'DGROOT' environment variable not set, exiting!" << std::endl;
	return 1;
    }

    if (runTests) {
	bool passed = runTestSuite();
	if (!passed) return 1;
    }
        
    std::string path;
    if (!pathArg.empty())
	path = pathArg;
    else
	path = bootFilePath();
        
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


/****************************************************************
 * Command line argument parsing.
 ****************************************************************/

/**
 * Invalid command line option error.
 */
void invalidOptionError(std::string& option) {
    std::cout << "Invalid option -- '" << option.substr(1) << "'"
	      << std::endl;
    throw 1;
}


/**
 * Invalid argument error.
 */
void invalidArgError(std::string& arg) {
    std::cout << "Invalid argument -- '" << arg << "'"
	      << std::endl;
    throw 1;
}


/**
 * Parse command line option.
 */
void parseOption(int argc, char* argv[], int index) {
    std::string arg = argv[index];

    if (arg == "-v") verbose = true;
    else if (arg == "-t") runTests = true;
    else invalidOptionError(arg);
}


/**
 * Parse command line arguments.
 * 
 * Returns source file path if provided,
 * otherwise the empty string.
 */
std::string parseArgs(int argc, char* argv[]) {
    std::string sourcePath = "";
    
    for (int i = 1; i < argc; i++) {
	std::string arg = argv[i];

	if (arg[0] == '-')
	    parseOption(argc, argv, i);
	else if (i == argc - 1)
	    sourcePath = arg;
	else
	    invalidArgError(arg);
    }

    return sourcePath;
}


/****************************************************************
 * Test suite.
 ****************************************************************/

/**
 * Run test suite.
 * Return true if all tests passed.
 */
bool runTestSuite() {
    Catch::Session session;
    return session.run() == 0;
}
