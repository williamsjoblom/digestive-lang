include_directories(.)

cmake_minimum_required(VERSION 3.6)
project(dig)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -rdynamic")

# ASMJIT
set(ASMJIT_EMBED TRUE)
add_definitions(-DASMJIT_STATIC)
set(ASMJIT_DIR "~/lib/asmjit")
include("${ASMJIT_DIR}/CMakeLists.txt")

include_directories("~/lib/asmjit/src/")
include_directories("~/lib/")

set(SOURCE_FILES main.cpp lexer/Lexer.cpp lexer/TokenQueue.cpp ast/BlockStmt.cpp ast/FunctionDecl.cpp ast/Decl.cpp ast/VariableDecl.cpp util/PrettyPrint.cpp parse/ParseError.cpp parse/Block.cpp parse/Function.cpp parse/Statement.cpp parse/File.cpp ast/BinaryExpr.cpp ast/Operator.cpp ast/ReturnStmt.cpp parse/Return.cpp parse/Variable.cpp ast/VariableExpr.cpp ast/FunctionCall.cpp semantic/Scope.cpp semantic/SemanticError.cpp ast/PlnStmt.cpp parse/Pln.cpp ast/IfStmt.cpp parse/If.cpp gen/Program.cpp gen/Expression.cpp gen/Return.cpp gen/Declaration.cpp ast/Unit.cpp parse/Unit.cpp gen/Function.cpp jit/JitContext.cpp interactive/IO.cpp interactive/Interactive.cpp jit/Jit.cpp ast/LoopStmt.cpp gen/Loop.cpp gen/BuiltIn.cpp jit/BuiltIn.cpp parse/Loop.cpp gen/If.cpp parse/Trap.cpp ast/UnaryExpr.cpp ast/type/Type.cpp ast/type/PrimitiveType.cpp ast/IntegerLiteral.cpp semantic/TypeUtil.cpp jit/Backtrace.cpp ast/type/TupleType.cpp parse/Type.cpp gen/BinaryExpr.cpp gen/Cast.cpp gen/Register.cpp parse/Expression.cpp ast/TupleExpr.cpp ast/type/DType.cpp gen/ErrHandler.cpp gen/CallConv.cpp ir/TAC.cpp ir/TACOp.cpp ir/TACEnv.cpp ir/TACType.cpp irgen/Expr.cpp irgen/Program.cpp irgen/BinaryExpr.cpp irgen/IfStmt.cpp irgen/VariableDecl.cpp)
add_executable(dig ${SOURCE_FILES} ${ASMJIT_SRC})

target_link_libraries(dig ${ASMJIT_DEPS})
