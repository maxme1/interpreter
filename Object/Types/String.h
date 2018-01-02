#ifndef INTERPRETER_STRING_H
#define INTERPRETER_STRING_H

#include <utility>

#include "../Exception.h"
#include "../Native/Native.h"


$class(String)
    std::string string{};
    String() = default;

    explicit String(std::string string) : string(std::move(string)) {}

    std::string asString() override {
        return string;
    }

    bool asBool() override {
        return !string.empty();
    }

    static std::string toString(ObjPtr object, Interpreter *interpreter) {
        auto aClass = std::dynamic_pointer_cast<Class>(object);
        if (aClass)
            return aClass->asString();
        auto method = object->findAttribute("str");
        if (!method)
            return object->asString();
        return interpreter->call(method, {})->asString();
    }

    $method(init, String)
        self->string = toString(positional[0], interpreter);
        return nullptr;
    }

    $method(add, String)
        auto that = cast(positional[0], true);
        return New(String(self->string + that->string));
    }

    static void populate() {
        addMethod("init", init, 1);
        addMethod("add", add, 1);
    }
};

#endif //INTERPRETER_STRING_H
