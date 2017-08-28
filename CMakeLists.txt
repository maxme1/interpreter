cmake_minimum_required(VERSION 3.7)
project(interpreter)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Tokenizer/Token.h Tokenizer/Tokenizer.cpp Tokenizer/Tokenizer.h Tokenizer/Tokenizer.cpp Parser/Expression/Expression.cpp Parser/Expression/Expression.h Parser/Parser.cpp Parser/Parser.h Object/Object.cpp Object/Object.h Interpreter/Interpreter.cpp Interpreter/Interpreter.h Parser/Statement/Statement.cpp Parser/Statement/Statement.h Object/Types/Int.h Object/Types/Bool.h Object/Types/None.h Object/Native/Native.h Object/Callable.h Object/Exception.h Interpreter/expressions.cpp Interpreter/statements.cpp Object/Class.h Object/Native/Native.cpp Object/Types/Array.h Object/Class.cpp Object/Callable.cpp Object/Native/NativeObject.h Object/Types/String.h Interpreter/API.h Object/Scope.h Object/Exception.cpp Interpreter/Builtins.cpp Object/Types/Range.h Object/Types/Array.cpp)
add_executable(interpreter ${SOURCE_FILES})