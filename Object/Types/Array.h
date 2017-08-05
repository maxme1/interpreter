#ifndef INTERPRETER_ARRAY_H
#define INTERPRETER_ARRAY_H

#include "../Native.h"
#include "Int.h"

struct Array : public ClassInstance {
    explicit Array(Object *classPtr) : ClassInstance(classPtr) {}

    std::vector<Object *> array;
};

struct ArrayClass : public NativeClass {
    ArrayClass() {
//        TODO: addMethod(name, func, args);
        methods["push"] = new NativeMethod(push, 1);
        methods["get"] = new NativeMethod(get, 1);
    }

    $method(push, Array)
        self->array.push_back(args[0]);
        return nullptr;
    }

    $method(get, Array)
        auto idx = Int::getInt(args[0]);
        if (idx >= self->array.size())
            throw Exception("IndexError");
        return self->array[idx];
    }

protected:
    Object *__call__(const std::vector<Object *> &args, Interpreter *interpreter) override {
        return new Array(this);
    }
};

#endif //INTERPRETER_ARRAY_H
