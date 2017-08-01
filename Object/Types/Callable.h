#ifndef INTERPRETER_FUNCTION_H
#define INTERPRETER_FUNCTION_H


#include "../Object.h"

class Callable : public Object {
    friend class Interpreter;
    Statement *body = nullptr;
    std::vector<std::string> arguments;
public:
    Callable() = default;

    Callable(const std::vector<std::string> &arguments) : arguments(arguments) {}

    Callable(const std::vector<std::string> &arguments, Statement *body) :
            body(body), arguments(arguments) {}

    ~Callable() {
        delete body;
    }

    virtual Object *call(Object *args) { return nullptr; };
};

#endif //INTERPRETER_FUNCTION_H
