#include <iostream>
#include <parse/Parse.h>
#include <lexer/Lexer.h>

#include "interactive/Interactive.h"

int main() {
    Jit jit;
    Interactive::start(&jit);
    
    if(jit.load("/home/wax/test.dg")) {
        int result = jit.run();

        return result;
    }

    return -1;
}
