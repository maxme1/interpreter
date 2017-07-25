#include <iostream>
#include <fstream>
#include "Interpreter/Interpreter.h"

int main() {
    Interpreter interpreter = Interpreter();
    std::ifstream source("../source.txt");
    if (source.good()) {
        std::string text((std::istreambuf_iterator<char>(source)),
                         (std::istreambuf_iterator<char>()));
        interpreter.interpret(text);
        source.close();
    } else std::cout << "Cannot open source file";
    return 0;
}