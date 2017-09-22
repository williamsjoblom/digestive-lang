#include <iostream>

#include "parse/Parse.h"
#include "lexer/Lexer.h"
#include "interactive/Interactive.h"

int main() {
    std::string path = "/home/wax/test.dg";
    
    Jit jit;
    Interactive::start(&jit);
    
    if(jit.load(path)) {
	std::cout << std::endl << "Run " << path << " => " << std::endl;
        int result = jit.run();
        return result;
    }
    
    return -1;
}
