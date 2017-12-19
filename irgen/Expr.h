#ifndef DIG_IRGEN_EXPR
#define DIG_IRGEN_EXPR
 
#include "../ir/TACEnv.h"
#include "../ir/TACOp.h"
#include "../ast/IntegerLiteral.h"


namespace Generate {
    TACOp integerLiteral(TACEnv& env, IntegerLiteral* e);
}


#endif
