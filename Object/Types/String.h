#ifndef INTERPRETER_STRING_H
#define INTERPRETER_STRING_H

#include <utility>

#include "../Exception.h"
#include "../Native/Native.h"

$class(String)

    std::string string{};
    String() = default;

    explicit String(std::string string) : string(std::move(string)) {}

    bool asBool() override {
        return !string.empty();
    }

    std::string asString() override {
        return string;
    }

    static std::string toString(ObjPtr object, Interpreter *interpreter) {
        return str(object, interpreter)->string;
    }

    static ptr str(ObjPtr object, Interpreter *interpreter) {
        auto method = object->findAttribute("str");
        if (method)
            return cast(interpreter->call(method, {}));
        return ptr(new String(object->asString()));
    }

    $method(init, String)
        self->string = str(positional[0], interpreter)->string;
        return nullptr;
    }

    $method(add, String)
        auto that = cast(positional[0], true);
        return New(String(self->string + that->string));
    }

    static void populate() {
        addMethod("init", init, 1);
        addMethod("plus", add, 1);
    }
};

#endif //INTERPRETER_STRING_H
