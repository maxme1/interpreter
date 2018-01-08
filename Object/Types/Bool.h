#ifndef INTERPRETER_BOOL_H
#define INTERPRETER_BOOL_H

#include "../Native/Native.h"

$class(Bool)

    bool value;
    Bool() = default;

    explicit Bool(bool value) : value(value) {}

    bool asBool() override {
        return value;
    }

    static bool toBool(ObjPtr object, Interpreter *interpreter) {
        auto method = object->findAttribute("bool");
        if (method)
            return interpreter->call(method, {})->asBool();
        return object->asBool();
    }

    $method(init, Bool)
        self->value = Bool::toBool(positional[0], interpreter);
        return nullptr;
    }

    $method(str, Bool)
        if (self->value)
            return New(String("True"));
        return New(String("False"));
    }

    static void populate() {
        addMethod("str", str);
        addMethod("init", init, 1);
    }
};

#endif //INTERPRETER_BOOL_H
