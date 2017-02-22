
#include <iostream>

#include "config.h"

#include "ast/type/Type.h"
#include "ast/type/Types.h"
#include "semantic/TypeUtil.h"
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