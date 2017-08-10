#ifndef INTERPRETER_STRING_H
#define INTERPRETER_STRING_H

#include <utility>

#include "../Native/Native.h"
#include "Exception.h"

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

    static std::string toString(Object *object, API *api) {
        auto aClass = dynamic_cast<Class *>(object);
        if (aClass)
            return aClass->asString();
        auto method = object->findAttribute("str");
        if (!method)
            return object->asString();
        return api->call(method, {})->asString();
    }

    $method(init, String)
        self->string = toString(args[0], api);
        return nullptr;
    }

    $method(add, String)
        auto that = dynamic_cast<String *>(args[0]);
        if (!that)
            throw Exception("not a string");
        return new String(self->string + that->string);
    }

    static void populate() {
        addMethod("init", init, 1);
        addMethod("add", add, 1);
    }
};

#endif //INTERPRETER_STRING_H
