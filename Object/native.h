#ifndef INTERPRETER_NATIVE_H
#define INTERPRETER_NATIVE_H

#include <iostream>
#include "Object.h"
#include "Types/None.h"
#include "Types/Callable.h"

class Print : public Callable {
public:
    Print() : Callable({"arg"}) {}

    Object *call(Object *args) {
        auto arg = args->getAttribute("arg");
        std::cout << arg->str() << std::endl;
        return nullptr;
    }
};

#endif //INTERPRETER_NATIVE_H
