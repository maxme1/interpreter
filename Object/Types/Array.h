#ifndef INTERPRETER_ARRAY_H
#define INTERPRETER_ARRAY_H

#include "../Native/Native.h"
#include "Int.h"

$class(Array)
    std::vector<ObjPtr> array;
    Array() = default;

    explicit Array(ArgsList array) : array(array) {}

    bool asBool() override {
        return !array.empty();
    }

    $method(init, Array)
        for (auto &&arg : args)
            self->array.push_back(arg);
        return nullptr;
    }

    $method(push, Array)
        self->array.push_back(args[0]);
        return nullptr;
    }

    $method(getItem, Array)
        auto idx = Int::getValue(args[0]);
        if (idx >= self->array.size())
            throw Wrap(new IndexError(std::to_string(idx)));
        return self->array[idx];
    }

    $method(size, Array)
        return New(Int(self->array.size()));
    }

    $method(setItem, Array)
        auto idx = Int::getValue(args[0]);
        if (idx >= self->array.size())
            throw Wrap(new IndexError(std::to_string(idx)));

        return self->array[idx] = args[1];
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
        return New(String(result + "]"));
    }

    static ObjPtr iter(ObjPtr _self, ArgsList args, API *api);

    static void populate() {
        addMethod("init", init, 0, true);
        addMethod("str", str);
        addMethod("push", push, 1);
        addMethod("setitem", setItem, 2);
        addMethod("getitem", getItem, 1);
        addMethod("iter", iter);

        addMethod("size", size);
    }
};

$class(IndexIterator)
    int index = -1;
    Array::ptr target;

    explicit IndexIterator(Array::ptr target) : target(std::move(target)) {}

    $method(iter, IndexIterator)
        return self;
    }

    $method(next, IndexIterator)
        self->index++;
        if (self->index >= self->target->array.size())
            throw Wrap(new StopIteration());
        return self->target->array[self->index];
    }

    static void populate() {
        addMethod("iter", iter);
        addMethod("next", next);
    }
};

#endif //INTERPRETER_ARRAY_H
