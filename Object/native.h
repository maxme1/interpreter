#ifndef INTERPRETER_NATIVE_H
#define INTERPRETER_NATIVE_H

#include <iostream>
#include "Types/Callable.h"

typedef Object *(*nativeFunction)(Object *);
class FromFunction : public Callable {
    std::vector<std::string> arguments;
    nativeFunction function;

protected:
    bool checkArguments(int count) override {
        return arguments.size() == count;
    }

    std::string argument(int i) override { return arguments[i]; }

    virtual Object *__call__(Object *args, Interpreter *interpreter) {
        return function(args);
    }

public:
    FromFunction(nativeFunction function, const std::vector<std::string> &arguments) :
            arguments(arguments), function(function) {}
};

#endif //INTERPRETER_NATIVE_H
