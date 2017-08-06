#ifndef INTERPRETER_ARRAY_H
#define INTERPRETER_ARRAY_H

#include "../Native/Native.h"
#include "Int.h"

$class(Array) {
    std::vector<Object *> array;

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

    static void populate() {
        addMethod("push", push, 1);
    }
};

#endif //INTERPRETER_ARRAY_H
