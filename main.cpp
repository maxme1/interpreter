#include <iostream>
#include "Interpreter/Interpreter.h"

int main() {
    Interpreter interpreter = Interpreter();
    interpreter.interpret("a = (1 + 6) / -2;");
    interpreter.interpret("a + 8; b = a = 1; a + b;");
    return 0;
}