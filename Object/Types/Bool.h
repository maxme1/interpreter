#ifndef INTERPRETER_BOOL_H
#define INTERPRETER_BOOL_H

#include "String.h"

$class(Bool) {
    bool value;
    Bool() = default;

    explicit Bool(bool value) : value(value) {}

    bool asBool() override {
        return value;
    }

    $method(str, Bool)
        if (self->value)
            return new String("True");
        return new String("False");
    }

    static void populate() {
        addMethod("str", str);
    }
};

#endif //INTERPRETER_BOOL_H
