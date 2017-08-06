#ifndef INTERPRETER_STRING_H
#define INTERPRETER_STRING_H

#include <utility>

#include "../Native/NativeObject.h"

struct String : public BaseNative {
    std::string string;

    explicit String(std::string string) : string(std::move(string)) {}

    std::string asString() override {
        return string;
    }

    bool asBool() override {
        return !string.empty();
    }

    Object *add(Object *other) override {
        auto that = dynamic_cast<String *>(other);
        if (!that)
            throw Exception("Not a string");
        return new String(string + that->string);
    }
};

struct StringClass : public Callable {
protected:
    bool checkArguments(int count) override {
        return count == 1;
    }

    Object *__call__(ArgsList args, Interpreter *interpreter) override {
        auto method = args[0]->findAttribute("str");
        if (method)
            return interpreter->callOperator(method, {});
        return new String(args[0]->asString());
    }
};

#endif //INTERPRETER_STRING_H
