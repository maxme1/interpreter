#include "Interpreter/Interpreter.h"

int main() {
    Interpreter interpreter = Interpreter();
    if (!interpreter.interpretFile("../source.txt"))
        std::cout << "Cannot open source file";
    return 0;
}