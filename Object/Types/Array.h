#ifndef INTERPRETER_ARRAY_H
#define INTERPRETER_ARRAY_H

#include "../Native/Native.h"
#include "Int.h"

$class(Array) {
    std::vector<Object *> array;

    bool asBool() override {
        return !array.empty();
    }

    $method(init, Array)
        for (auto &&arg : args) {
            arg->save();
            self->array.push_back(arg);
        }
    }

    $method(push, Array)
        args[0]->save();
        self->array.push_back(args[0]);
        return nullptr;
    }

    $method(getItem, Array)
        auto idx = Int::getValue(args[0]);
        if (idx >= self->array.size())
            throw Exception("IndexError");
        return self->array[idx];
    }

    $method(setItem, Array)
        auto idx = Int::getValue(args[0]);
        if (idx >= self->array.size())
            throw Exception("IndexError");

        args[1]->save();
        auto old = self->array[idx];
        if (old->canDelete())
            delete old;

        self->array[idx] = args[1];
        return args[1];
    }

    $method(str, Array)
        std::string result = "[";
        bool first = true;
        for (auto &&item : self->array) {
            if (not first)
                result += ", ";
            else
                first = false;
            result += String::toString(item, api);
        }
        return new String(result + "]");
    }

    static void populate() {
        addMethod("init", init, 0, -1);
        addMethod("push", push, 1);
        addMethod("setitem", setItem, 2);
        addMethod("getitem", getItem, 1);
        addMethod("str", str, 0);
    }
};

#endif //INTERPRETER_ARRAY_H
