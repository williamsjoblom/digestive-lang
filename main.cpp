
#include <iostream>
#include <asmjit/asmjit.h>
#include <unistd.h>
#include <jit/JitContext.h>


#include "config.h"

#include "gen/Gen.h"
#include "lexer/Lexer.h"
#include "parse/Parse.h"
#include "interactive/Interactive.h"
#include "jit/Jit.h"

int main() {
    Jit jit;
    Interactive::start(&jit);

    jit.load("/home/wax/fib.dg");

    int result = jit.run();

    return result;
}