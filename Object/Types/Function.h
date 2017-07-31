#ifndef INTERPRETER_FUNCTION_H
#define INTERPRETER_FUNCTION_H

#include "../Object.h"

class Function : public Object {
    friend class Interpreter;
public:
    explicit Function(const std::vector<std::string> &arguments, Statement *body) {
        functionArguments = arguments;
        functionBody = body;
    }
};

#endif //INTERPRETER_FUNCTION_H
