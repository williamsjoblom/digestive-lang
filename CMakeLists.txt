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

set(SOURCE_FILES main.cpp lexer/Lexer.cpp lexer/TokenQueue.cpp ast/BlockStmt.cpp ast/FunctionDecl.cpp ast/Decl.cpp ast/VariableDecl.cpp util/PrettyPrint.cpp parse/ParseError.cpp parse/Block.cpp parse/Function.cpp parse/Statement.cpp parse/File.cpp ast/BinaryExpr.cpp ast/Operator.cpp ast/ReturnStmt.cpp parse/Return.cpp parse/Variable.cpp ast/VariableExpr.cpp ast/FunctionCall.cpp semantic/Scope.cpp semantic/SemanticError.cpp ast/PlnStmt.cpp parse/Pln.cpp ast/IfStmt.cpp parse/If.cpp ast/Unit.cpp parse/Unit.cpp jit/JitContext.cpp interactive/IO.cpp interactive/Interactive.cpp jit/Jit.cpp ast/LoopStmt.cpp jit/BuiltIn.cpp parse/Loop.cpp parse/Trap.cpp ast/UnaryExpr.cpp ast/type/Type.cpp ast/type/PrimitiveType.cpp ast/IntegerLiteral.cpp semantic/TypeUtil.cpp jit/Backtrace.cpp ast/type/TupleType.cpp parse/Type.cpp parse/Expression.cpp ast/TupleExpr.cpp ast/type/DType.cpp ir/TAC.cpp ir/TACOp.cpp ir/TACFun.cpp ir/TACType.cpp genir/Expr.cpp genir/Program.cpp genir/BinaryExpr.cpp genir/IfStmt.cpp genir/VariableDecl.cpp genir/Function.cpp ir/TACProgram.cpp util/File.cpp genir/Return.cpp genir/FunctionCall.cpp genir/BuiltIn.cpp genasm/TACCompiler.cpp genasm/Instr.cpp genasm/Op.cpp genasm/InstrEnv.cpp util/BuildTimestamp.cpp genir/LoopStmt.cpp genir/Trap.cpp genasm/ErrHandler.cpp parse/earley/BNF.cpp parse/earley/Expr.cpp)
add_executable(dig ${SOURCE_FILES} ${ASMJIT_SRC})

target_link_libraries(dig ${ASMJIT_DEPS})

# Force recompilation of Timestamp.cpp to update __TIME__ and __DATE__ macros.
add_custom_target(
    touchTimestamp
    COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/util/BuildTimestamp.cpp
)
add_dependencies(dig touchTimestamp)
