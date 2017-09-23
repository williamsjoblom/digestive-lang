#include <iostream>

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
        int result = jit.run();
        return result;
    }
    
    return -1;
}
