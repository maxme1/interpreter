#ifndef INTERPRETER_ARRAY_H
#define INTERPRETER_ARRAY_H

#include "../Native/Native.h"
#include "Int.h"

$class(Array)
    std::vector<ObjPtr> array;
    Array() = default;

    Array(ArgsList array) : array(array) {}

    bool asBool() override {
        return !array.empty();
    }

    $method(init, Array)
        for (auto &&arg : positional)
            self->array.push_back(arg);
        return nullptr;
    }

    $method(push, Array)
        self->array.push_back(positional[0]);
        return nullptr;
    }

    $method(getItem, Array)
        auto idx = Int::toInt(positional[0]);
        if (idx >= self->array.size())
            assert(false);
//            throw Wrap(new IndexError(std::to_string(idx)));
        return self->array[idx];
    }

    $method(setItem, Array)
        auto idx = Int::toInt(positional[0]);
        if (idx >= self->array.size())
            assert(false);
//            throw Wrap(new IndexError(std::to_string(idx)));

        return self->array[idx] = positional[1];
    }

    $method(str, Array)
        std::string result = "[";
        bool first = true;
        for (auto &&item : self->array) {
            if (not first)
                result += ", ";
            else
                first = false;
            result += String::str(item, interpreter)->string;
        }
        return New(String(result + "]"));
    }

    static void populate() {
        addMethod("init", init, 0);
        addMethod("str", str, 0);
        addMethod("push", push, 1);
        addMethod("setitem", setItem, 2);
        addMethod("getitem", getItem, 1);
    }
};

#endif //INTERPRETER_ARRAY_H
