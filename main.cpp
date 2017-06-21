
#include <iostream>

#include "interactive/Interactive.h"

int main() {
    Jit jit;
    Interactive::start(&jit);


    jit.load("/home/wax/test.dg");

    int result = jit.run();
    
    return result;
}