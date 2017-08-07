#ifndef INTERPRETER_STRING_H
#define INTERPRETER_STRING_H

#include <utility>

#include "../Native/Native.h"

$class(String) {
    std::string string{};
    String() = default;

    explicit String(std::string string) : string(std::move(string)) {}

    std::string asString() override {
        return string;
    }

    static std::string toString(Object *object, API *api) {
        auto method = object->findAttribute("str");
        if (!method)
            return object->asString();
        return api->call(method, {})->asString();
    }

    bool asBool() override {
        return !string.empty();
    }

    $method(add, String)
        auto that = dynamic_cast<String *>(args[0]);
        if (!that)
            throw Exception("not a string");
        return new String(self->string + that->string);
    }

    $method(init, String)
        self->string = toString(args[0], api);
        return nullptr;
    }

    static void populate() {
        addMethod("init", init, 1);
        addMethod("add", add, 1);
    }
};

#endif //INTERPRETER_STRING_H
