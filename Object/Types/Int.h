#ifndef INTERPRETER_INT_H
#define INTERPRETER_INT_H

#include "Exception.h"
#include "../Native/Native.h"
#include "String.h"
#include "Bool.h"

$class(Int) {
    int value;
    Int() = default;

    explicit Int(int value) : value(value) {}

    static int getValue(Object *object) {
        return cast(object, true)->value;
    }

    bool asBool() override {
        return value != 0;
    }

    std::string asString() override {
        return std::to_string(value);
    }

    $method(init, Int)
        self->value = Int::getValue(args[0]);
        return nullptr;
    }

    $method(str, Int)
        return new String(self->asString());
    }

    $method(add, Int)
        return new Int(self->value + Int::getValue(args[0]));
    }

    $method(divide, Int)
        int val = Int::getValue(args[0]);
        if (val == 0)
            throw Exception("Division by zero");
        return new Int(self->value / val);
    }

    $method(unary_add, Int)
        return self;
    }

    $method(unary_subtract, Int)
        return new Int(-self->value);
    }

    $method(multiply, Int)
        return new Int(self->value * Int::getValue(args[0]));
    }

    $method(subtract, Int)
        return new Int(self->value - Int::getValue(args[0]));
    }

    $method(equal, Int)
        auto other = Int::cast(args[0]);
        if (other)
            return new Bool(self->value == other->value);
        return new Bool(false);
    }

    $method(not_equal, Int)
        auto other = Int::cast(args[0]);
        if (other)
            return new Bool(self->value != other->value);
        return new Bool(false);
    }

    $method(greater, Int)
        return new Int(self->value > Int::getValue(args[0]));
    }

    $method(less, Int)
        return new Int(self->value < Int::getValue(args[0]));
    }

    $method(greater_or_equal, Int)
        return new Int(self->value >= Int::getValue(args[0]));
    }

    $method(less_or_equal, Int)
        return new Int(self->value <= Int::getValue(args[0]));
    }

    static void populate() {
//        TODO: populate
        addMethod("init", init, 1);
        addMethod("str", str, 0);
        addMethod("add", add, 1);
        addMethod("div", divide, 1);
        addMethod("eq", equal, 1);
    }
};


#endif //INTERPRETER_INT_H
