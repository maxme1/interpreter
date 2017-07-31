#ifndef INTERPRETER_NATIVE_H
#define INTERPRETER_NATIVE_H

#include <iostream>
#include "Object.h"
#include "Types/None.h"

class Print : public Object {
public:
    Print() {
        functionArguments.push_back("arg");
    }

    Object *__call__(Object *args) override {
        auto arg = args->getAttribute("arg");
        std::cout << arg->str() << std::endl;
        return nullptr;
    }
};

#endif //INTERPRETER_NATIVE_H
