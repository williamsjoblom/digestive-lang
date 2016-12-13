cmake_minimum_required(VERSION 3.6)
project(dig)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp lexer/Lexer.cpp lexer/Lexer.h lexer/TokenQueue.cpp lexer/TokenQueue.h lexer/Token.h)
add_executable(dig ${SOURCE_FILES})