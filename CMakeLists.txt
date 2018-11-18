include_directories(.)

cmake_minimum_required(VERSION 3.5)
project(dg)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -rdynamic")

# ASMJIT
set(ASMJIT_EMBED TRUE)
add_definitions(-DASMJIT_STATIC)
set(ASMJIT_DIR "./asmjit/")
include("${ASMJIT_DIR}/CMakeLists.txt")

include_directories("${ASMJIT_DIR}/src/")

set(
  SOURCE_FILES
  main.cpp

  # AST
  ast/construct.cpp
  
  ast/BlockStmt.cpp
  ast/FunctionDecl.cpp
  ast/Decl.cpp
  ast/VariableDecl.cpp
  ast/BinaryExpr.cpp
  ast/Operator.cpp
  ast/ReturnStmt.cpp
  ast/VariableExpr.cpp
  ast/FunctionCall.cpp
  ast/PlnStmt.cpp
  ast/IfStmt.cpp
  ast/Unit.cpp
  ast/LoopStmt.cpp
  ast/UnaryExpr.cpp
  ast/type/Type.cpp
  ast/type/PrimitiveType.cpp
  ast/IntegerLiteral.cpp
  ast/type/TupleType.cpp
  ast/TupleExpr.cpp
  ast/type/DType.cpp

  # Assembly generator
  genasm/TACCompiler.cpp
  genasm/Instr.cpp
  genasm/Op.cpp
  genasm/InstrEnv.cpp
  genasm/ErrHandler.cpp

  # IR generator
  genir/Expr.cpp
  genir/Program.cpp
  genir/BinaryExpr.cpp
  genir/IfStmt.cpp
  genir/VariableDecl.cpp
  genir/Function.cpp
  genir/Return.cpp
  genir/FunctionCall.cpp
  genir/BuiltIn.cpp
  genir/LoopStmt.cpp
  genir/Trap.cpp

  
  # Interactive
  interactive/IO.cpp
  interactive/Interactive.cpp
  
  # IR
  ir/TAC.cpp
  ir/TACOp.cpp
  ir/TACFun.cpp
  ir/TACType.cpp
  ir/TACProgram.cpp
  
  # JIT
  jit/JitContext.cpp    
  jit/Jit.cpp
  jit/BuiltIn.cpp
  jit/Backtrace.cpp

  # Lexer
  lexer/Lexer.cpp
  lexer/TokenQueue.cpp

  # Parser
  parse/ParseError.cpp
  parse/Block.cpp
  parse/Function.cpp
  parse/Statement.cpp
  parse/File.cpp
  parse/Return.cpp
  parse/Variable.cpp
  parse/Pln.cpp
  parse/If.cpp
  parse/Unit.cpp
  parse/Loop.cpp
  parse/Trap.cpp
  parse/Type.cpp
  parse/Expression.cpp
  parse/earley/BNF.cpp
  parse/earley/Parse.cpp
  parse/earley/BNFParser.cpp
  parse/earley/Chart.cpp
  parse/earley/ASTNode.cpp

  # Semantics
  semantic/Scope.cpp
  semantic/SemanticError.cpp
  semantic/TypeUtil.cpp

  # Utilities
  util/File.cpp
  util/BuildTimestamp.cpp
  util/Path.cpp
  util/Hash.cpp
  util/PrettyPrint.cpp
  util/tty/Tree.cpp
  util/tty/Color.cpp
)
add_executable(dg ${SOURCE_FILES} ${ASMJIT_SRC})

target_link_libraries(dg ${ASMJIT_DEPS})

# Force recompilation of BuildTimestamp.cpp to update __TIME__ and __DATE__ macros.
add_custom_target(
    touchTimestamp
    COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/util/BuildTimestamp.cpp
)
add_dependencies(dg touchTimestamp)

# Ugly hack to move compile_commands to the project root
ADD_CUSTOM_TARGET(
	moveCompileCommands
	cp compile_commands.json ..
)
add_dependencies(dg moveCompileCommands)
