#ifndef DIG_IRGEN_EXPR
#define DIG_IRGEN_EXPR
 
#include "ir/TAC.h"
#include "ir/TACOp.h"
#include "ast/IntegerLiteral.h"
#include "ast/VariableExpr.h"


namespace Generate {
    TACOp integerLiteral(TACFun* env, IntegerLiteral* e);
    TACOp variable(TACFun* env, VariableExpr* e);
}


#endif
