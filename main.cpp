

#include <iostream>
#include <parse/Parse.h>
#include <lexer/Lexer.h>

#include "interactive/Interactive.h"

int main() {
    /*
    Lexer l = Lexer();
    TokenQueue tokens = l.lex("3 * (3 + 7);");
    Parse::expression(tokens)->dump();

    return 0;
*/

    Jit jit;
    Interactive::start(&jit);


    //FIXME: decl gets wrong regsize.

    if(jit.load("/home/wax/test.dg")) {
        int result = jit.run();

        return result;
    }

    return -1;
}