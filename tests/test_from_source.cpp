#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../TreeWalk/Interpreter/Interpreter.h"

TEST_CASE("test") {
    Interpreter interpreter = Interpreter();

    std::stringstream buffer;
    std::streambuf *sbuf = std::cout.rdbuf();

    std::cout.rdbuf(buffer.rdbuf());

    REQUIRE_NOTHROW(interpreter.interpret("var a = 1; print(a);\n"));
    REQUIRE(buffer.str() == "1\n");

    std::cout.rdbuf(sbuf);
}
