#include <iostream>
#include <ctime>

#include "asmjit/asmjit.h"
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include "options.hh"
#include "util/BuildTimestamp.hh"
#include "parse/Parse.hh"
#include "lexer/Lexer.hh"
#include "interactive/Interactive.hh"
#include "util/tty/Tree.hh"

#include "ir/TACProgram.hh"
#include "genir/Program.hh"
#include "util/File.hh"
#include "util/Path.hh"
#include "genasm/TACCompiler.hh"
#include "parse/earley/BNFParser.hh"
#include "parse/earley/BNF.hh"
#include "parse/earley/Parse.hh"
#include "parse/earley/ASTNode.hh"

/****************************************************************
 * Globals defined as 'extern' in 'options.h'.
 ****************************************************************/
bool verbose { false };
bool runTests { false };

/**
 * Dump.
 */
bool dumpParseDesugaredGrammar { false };
bool dumpParseSets { false };
bool dumpParseStateTree { false };
bool dumpJitHandles { false };


/**
 * Forward declarations.
 */
std::string parseArgs(int argc, char* argv[]);
bool runTestSuite();

/**
 * Main.
 */
int main(int argc, char* argv[]) {    
    std::string pathArg { parseArgs(argc, argv) };

    if (verbose) {
	std::cout << " digestive " << version
		  << ", " << buildTimestamp() << std::endl;
    }

    
    if (!hasRootDirPath()) {
	std::cerr << "'DGROOT' environment variable not set, exiting!" << std::endl;
	return 1;
    }

    if (runTests) {
	bool passed { runTestSuite() };
	if (!passed) return 1;
    }
        
    std::string path;
    if (!pathArg.empty())
	path = pathArg;
    else
	path = bootFilePath();
        
    Jit jit;
    Interactive::start(&jit);

    std::clock_t compile_t0 { std::clock() };

    bool loaded = jit.load(path);

    std::clock_t compile_t1 { std::clock() };
    double compile_dt { double(compile_t1 - compile_t0) / (CLOCKS_PER_SEC / 1000) };
    
    if (verbose)
	std::cout << "Compilation in " << compile_dt << " ms" << std::endl;
    
    if(loaded) {
	if (verbose)
	    std::cout << "Run " << path << " => " << std::endl;

	std::clock_t run_t0 { std::clock() };
	
	int result { jit.run() };
	
	std::clock_t run_t1 { std::clock() };
	double run_dt { double(run_t1 - run_t0) / (CLOCKS_PER_SEC / 1000) };
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
[[noreturn]] void invalidOptionError(std::string& option) {
    std::cout << "Invalid option -- '" << option.substr(1) << "'"
	      << std::endl;
    throw 1;
}


/**
 * Invalid argument error.
 */
[[noreturn]] void invalidArgError(std::string& arg) {
    std::cout << "Invalid argument -- '" << arg << "'"
	      << std::endl;
    throw 1;
}


/**
 * Option without argument error.
 */
[[noreturn]] void expectedOptionArgError(std::string& option) {
    std::cout << "Expected argument for option -- '" << option << "'"
	      << std::endl;
    throw 1;
}


/**
 * Parse command line option.
 */
int parseOption(int argc, char* argv[], int index) {
    std::string option { argv[index] };

    if (option == "-v" ||
	option == "--verbose") {
	verbose = true;
	return 1;
    } else if (option == "-t" ||
	       option == "--test") {
	runTests = true;
	return 1;
    } else if (option == "-d" ||
	       option == "--dump") {
	int argIndex = index + 1;
	if (argc <= argIndex)
	    expectedOptionArgError(option);
	
	std::string arg = argv[argIndex];
	if (arg == "bnf")
	    dumpParseDesugaredGrammar = true;
	else if (arg == "ps")
	    dumpParseSets = true;
	else if (arg == "pst")
	    dumpParseStateTree = true;
	else if (arg == "h")
	    dumpJitHandles = true;
	else
	    invalidArgError(arg);
	
	return 2;
    } else {
	invalidOptionError(option);
    }
}


/**
 * Parse command line arguments.
 * 
 * Returns source file path if provided,
 * otherwise the empty string.
 */
std::string parseArgs(int argc, char* argv[]) {
    std::string sourcePath = "";

    int i { 1 };
    while (i < argc) {
	std::string arg = argv[i];

	if (arg[0] == '-') {
	    i += parseOption(argc, argv, i);
	} else if (i == argc - 1) {
	    sourcePath = arg;
	    i++;
	} else {
	    invalidArgError(arg);
	}
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
