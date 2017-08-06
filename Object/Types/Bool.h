#ifndef INTERPRETER_BOOL_H
#define INTERPRETER_BOOL_H

#include <string>
#include "../Object.h"

class Bool : public Object {
public:
    bool asBool() override {
        return value;
    }

    bool value;

    explicit Bool(bool value) : value(value) {}

    std::string asString() {
        if (value)
            return "True";
        return "False";
    }
};

#endif //INTERPRETER_BOOL_H
