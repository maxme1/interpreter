#ifndef INTERPRETER_ARRAY_H
#define INTERPRETER_ARRAY_H

#include "../Native/Native.h"
#include "Int.h"

// TODO: memory management
$class(Array) {
    std::vector<Object *> array;

    $method(init, Array)
        for (auto &&arg : args) {
            self->array.push_back(arg);
        }
    }

    $method(push, Array)
        self->array.push_back(args[0]);
        return nullptr;
    }

    Object *getItem(Object *other) override {
        auto idx = Int::getInt(other);
        if (idx >= this->array.size())
            throw Exception("IndexError");
        return this->array[idx];
    }

    Object *setItem(Object *other, Object *value) override {
        auto idx = Int::getInt(other);
        if (idx >= this->array.size())
            throw Exception("IndexError");
        this->array[idx] = value;
        return value;
    }

    std::string asString() override {
        std::string result = "[";
        bool first = true;
        for (auto &&item : array) {
            if (not first)
                result += ", ";
            else
                first = false;
            result += item->asString();
        }
        return result + "]";
    }

    static void populate() {
        addMethod("init", init, 1, -1);
        addMethod("push", push, 1);
    }
};

#endif //INTERPRETER_ARRAY_H
