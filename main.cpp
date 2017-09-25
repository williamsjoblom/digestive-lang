#include <iostream>
#include <ctime>

#include "parse/Parse.h"
#include "lexer/Lexer.h"
#include "interactive/Interactive.h"

int main(int argc, char* argv[]) {
    std::string path;
    if (argc == 1)
	path = "/home/wax/test.dg";
    else
	path = argv[1];
    
    Jit jit;
    Interactive::start(&jit);
    
    if(jit.load(path)) {
	std::cout << std::endl << "Run " << path << " => " << std::endl;

	std::clock_t begin = std::clock();

	int result = jit.run();

	std::clock_t end = std::clock();
	double elapsed = double(end - begin) / (CLOCKS_PER_SEC / 1000);
       	std::cout << "Completed in " << elapsed << " ms" << std::endl;
	
        return result;
    }
    
    return -1;
}
