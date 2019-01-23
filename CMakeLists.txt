include_directories(.)

cmake_minimum_required(VERSION 3.5)
project(dg)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -rdynamic")

# ASMJIT
set(ASMJIT_EMBED TRUE)
add_definitions(-DASMJIT_STATIC)
set(ASMJIT_DIR "./asmjit/")
include("${ASMJIT_DIR}/CMakeLists.txt")

include_directories("${ASMJIT_DIR}/src/")

set(
  SOURCE_FILES
  main.cc

  # AST
  ast/BlockStmt.cc
  ast/FunctionDecl.cc
  ast/Decl.cc
  ast/VariableDecl.cc
  ast/BinaryExpr.cc
  ast/Operator.cc
  ast/ReturnStmt.cc
  ast/VariableExpr.cc
  ast/FunctionCall.cc
  ast/PlnStmt.cc
  ast/IfStmt.cc
  ast/Unit.cc
  ast/LoopStmt.cc
  ast/UnaryExpr.cc
  ast/type/Type.cc
  ast/type/PrimitiveType.cc
  ast/IntegerLiteral.cc
  ast/type/TupleType.cc
  ast/TupleExpr.cc
  ast/type/DType.cc

  # Assembly generator
  genasm/TACCompiler.cc
  genasm/Instr.cc
  genasm/Op.cc
  genasm/InstrEnv.cc
  genasm/ErrHandler.cc

  # IR generator
  genir/Expr.cc
  genir/Program.cc
  genir/BinaryExpr.cc
  genir/IfStmt.cc
  genir/VariableDecl.cc
  genir/Function.cc
  genir/Return.cc
  genir/FunctionCall.cc
  genir/BuiltIn.cc
  genir/LoopStmt.cc
  genir/Trap.cc

  
  # Interactive
  interactive/IO.cc
  interactive/Interactive.cc
  
  # IR
  ir/TAC.cc
  ir/TACOp.cc
  ir/TACFun.cc
  ir/TACType.cc
  ir/TACProgram.cc
  
  # JIT
  jit/JitContext.cc    
  jit/Jit.cc
  jit/BuiltIn.cc
  jit/Backtrace.cc

  # Lexer
  lexer/Lexer.cc
  lexer/TokenQueue.cc

  # Parser
  parse/ParseError.cc
  parse/Block.cc
  parse/Function.cc
  parse/Statement.cc
  parse/File.cc
  parse/Return.cc
  parse/Variable.cc
  parse/Pln.cc
  parse/If.cc
  parse/Unit.cc
  parse/Loop.cc
  parse/Trap.cc
  parse/Type.cc
  parse/Expression.cc
  parse/earley/BNF.cc
  parse/earley/Parse.cc
  parse/earley/BNFParser.cc
  parse/earley/Chart.cc
  parse/earley/ASTNode.cc
  parse/earley/construct.cc

  # Semantics
  semantic/Scope.cc
  semantic/SemanticError.cc
  semantic/TypeUtil.cc

  # Utilities
  util/File.cc
  util/BuildTimestamp.cc
  util/Path.cc
  util/Hash.cc
  util/PrettyPrint.cc
  util/tty/Tree.cc
  util/tty/Color.cc
)
add_executable(dg ${SOURCE_FILES} ${ASMJIT_SRC})

target_link_libraries(dg ${ASMJIT_DEPS})

# Force recompilation of BuildTimestamp.cc to update __TIME__ and __DATE__ macros.
add_custom_target(
    touchTimestamp
    COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/util/BuildTimestamp.cc
)
add_dependencies(dg touchTimestamp)

# Ugly hack to move compile_commands to the project root
ADD_CUSTOM_TARGET(
	moveCompileCommands
	cp compile_commands.json ..
)
add_dependencies(dg moveCompileCommands)
