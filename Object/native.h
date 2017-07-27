#ifndef INTERPRETER_NATIVE_H
#define INTERPRETER_NATIVE_H

#include <iostream>
#include "Object.h"
#include "Types/None.h"

class Print : public Object {
public:
    Object *__call__(Object *other) override {
        std::cout << other->str() << std::endl;
        return new None();
    }
};


#endif //INTERPRETER_NATIVE_H
