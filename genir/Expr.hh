#pragma once

#include "ast/IntegerLiteral.hh"
#include "ast/VariableExpr.hh"

/**
 * Forward declarations.
 */
class TACFun;
class IntegerLiteral;
class VariableExpr;
class TupleExpr;


namespace Generate {
    TACOp integerLiteral(TACFun* fun, IntegerLiteral* e);
    TACOp variable(TACFun* fun, VariableExpr* e);
    TACOp tuple(TACFun* fun, TupleExpr* e);
}
