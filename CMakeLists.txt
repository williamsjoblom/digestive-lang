
include_directories(.)

cmake_minimum_required(VERSION 3.6)
project(dig)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp lexer/Lexer.cpp lexer/Lexer.h lexer/TokenQueue.cpp lexer/TokenQueue.h lexer/Token.h ast/BlockStmt.cpp ast/BlockStmt.h ast/Expr.h ast/Node.h ast/Stmt.h util/PrettyPrint.h parse/ParseError.h ast/FunctionDecl.cpp ast/FunctionDecl.h ast/Decl.cpp ast/Decl.h ast/VariableDecl.cpp ast/VariableDecl.h util/PrettyPrint.cpp parse/ParseError.cpp parse/Expression.cpp parse/Parse.h parse/Block.cpp parse/Function.cpp parse/Statement.cpp parse/File.cpp ast/BinaryExpr.cpp ast/BinaryExpr.h ast/LiteralExpr.cpp ast/LiteralExpr.h ast/Operator.cpp ast/Operator.h ast/ExprStmt.h ast/ReturnStmt.cpp ast/ReturnStmt.h parse/Return.cpp parse/Variable.cpp ast/VariableExpr.cpp ast/VariableExpr.h ast/FunctionCall.cpp ast/FunctionCall.h ast/Analyzable.h ast/Scope.cpp ast/Scope.h)
add_executable(dig ${SOURCE_FILES})