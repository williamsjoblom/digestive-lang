include_directories(.)

cmake_minimum_required(VERSION 3.6)
project(dig)

set(CMAKE_CXX_STANDARD 11)

# ASMJIT
set(ASMJIT_EMBED TRUE)
add_definitions(-DASMJIT_STATIC)
set(ASMJIT_DIR "~/lib/asmjit")
include("${ASMJIT_DIR}/CMakeLists.txt")

set(SOURCE_FILES main.cpp lexer/Lexer.cpp lexer/Lexer.h lexer/TokenQueue.cpp lexer/TokenQueue.h lexer/Token.h ast/BlockStmt.cpp ast/BlockStmt.h ast/Expr.h ast/Node.h ast/Stmt.h util/PrettyPrint.h parse/ParseError.h ast/FunctionDecl.cpp ast/FunctionDecl.h ast/Decl.cpp ast/Decl.h ast/VariableDecl.cpp ast/VariableDecl.h util/PrettyPrint.cpp parse/ParseError.cpp parse/Expression.cpp parse/Parse.h parse/Block.cpp parse/Function.cpp parse/Statement.cpp parse/File.cpp ast/BinaryExpr.cpp ast/BinaryExpr.h ast/Literal.h ast/Operator.cpp ast/Operator.h ast/ExprStmt.h ast/ReturnStmt.cpp ast/ReturnStmt.h parse/Return.cpp parse/Variable.cpp ast/VariableExpr.cpp ast/VariableExpr.h ast/FunctionCall.cpp ast/FunctionCall.h ast/Analyzable.h semantic/Scope.cpp semantic/Scope.h semantic/SemanticError.cpp semantic/SemanticError.h ast/PlnStmt.cpp ast/PlnStmt.h parse/Pln.cpp ast/IfStmt.cpp ast/IfStmt.h parse/If.cpp gen/Gen.h gen/Program.cpp gen/Expression.cpp gen/Return.cpp config.h gen/Declaration.cpp ast/Unit.cpp ast/Unit.h parse/Unit.cpp gen/Function.cpp jit/JitContext.h jit/JitContext.cpp interactive/IO.cpp interactive/IO.h interactive/Interactive.cpp interactive/Interactive.h jit/Jit.cpp jit/Jit.h ast/LoopStmt.cpp ast/LoopStmt.h gen/Loop.cpp gen/BuiltIn.cpp jit/BuiltIn.cpp jit/BuiltIn.h parse/Loop.cpp util/Colors.h gen/If.cpp ast/Trap.h parse/Trap.cpp ast/UnaryExpr.cpp ast/UnaryExpr.h ast/type/Type.cpp ast/type/Type.h ast/type/PrimitiveType.cpp ast/type/PrimitiveType.h ast/type/Types.h ast/IntegerLiteral.cpp ast/IntegerLiteral.h semantic/TypeUtil.cpp semantic/TypeUtil.h ast/type/TypeVisitor.h ast/type/Types.cpp ast/RealLiteral.cpp ast/RealLiteral.h parse/Type.cpp)
add_executable(dig ${SOURCE_FILES} ${ASMJIT_SRC})

target_link_libraries(dig ${ASMJIT_DEPS})
