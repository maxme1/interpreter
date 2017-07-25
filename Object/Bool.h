#ifndef INTERPRETER_BOOL_H
#define INTERPRETER_BOOL_H

#include <string>
#include "Object.h"

class Bool : public Object {
public:
    bool value;

    Bool(bool value) : value(value) {}

    std::string str() {
        if (value)
            return "True";
        return "False";
    }
};

#endif //INTERPRETER_BOOL_H
