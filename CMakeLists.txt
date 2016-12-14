
include_directories(.)

cmake_minimum_required(VERSION 3.6)
project(dig)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp lexer/Lexer.cpp lexer/Lexer.h lexer/TokenQueue.cpp lexer/TokenQueue.h lexer/Token.h ast/BlockStmt.cpp ast/BlockStmt.h ast/Expr.h ast/Node.h ast/Stmt.h parse/Parser.h parse/StmtParser.h util/PrettyPrint.h parse/BlockStmtParser.h parse/ParseError.h ast/FunctionDecl.cpp ast/FunctionDecl.h ast/Decl.cpp ast/Decl.h ast/VariableDecl.cpp ast/VariableDecl.h parse/FunctionParser.h util/PrettyPrint.cpp parse/ParseError.cpp)
add_executable(dig ${SOURCE_FILES})