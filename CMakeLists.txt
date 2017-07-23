cmake_minimum_required(VERSION 3.7)
project(interpreter)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Tokenizer/Token.cpp Tokenizer/Token.h Tokenizer/Tokenizer.cpp Tokenizer/Tokenizer.h Tokenizer/Tokenizer.cpp Parser/Expression/Expression.cpp Parser/Expression/Expression.h Parser/Parser.cpp Parser/Parser.h Object/Object.cpp Object/Object.h Interpreter/Interpreter.cpp Interpreter/Interpreter.h Parser/Statement/Statement.cpp Parser/Statement/Statement.h)
add_executable(interpreter ${SOURCE_FILES})