#ifndef INTERPRETER_INT_H
#define INTERPRETER_INT_H

#include <utility>

#include "../Exception.h"
#include "../Native/Native.h"
#include "String.h"
#include "Bool.h"

$class(Int)
    long value = 0;
    Int() = default;

    explicit Int(long value) : value(value) {}

    static long getValue(ObjPtr object) {
        return cast(std::move(object), true)->value;
    }

    bool asBool() override {
        return value != 0;
    }

    std::string asString() override {
        return std::to_string(value);
    }

    $method(init, Int)
        if (cast(args[0]))
            self->value = Int::getValue(args[0]);
        else if (String::cast(args[0]))
            self->value = std::stoi(args[0]->asString());
        else if (Bool::cast(args[0]))
            self->value = (int) args[0]->asBool();
        else throw Wrap(new ValueError("Could not convert to Int"));
        return nullptr;
    }

    $method(str, Int)
        return New(String(self->asString()));
    }

    $method(add, Int)
        return New(Int(self->value + Int::getValue(args[0])));
    }

    $method(divide, Int)
        auto val = Int::getValue(args[0]);
        if (val == 0)
            throw Wrap(new Exception("Division by zero"));
        return New(Int(self->value / val));
    }

    $method(unary_add, Int)
        return New(Int(self->value));
    }

    $method(unary_subtract, Int)
        return New(Int(-self->value));
    }

    $method(multiply, Int)
        return New(Int(self->value * Int::getValue(args[0])));
    }

    $method(subtract, Int)
        return New(Int(self->value - Int::getValue(args[0])));
    }

    $method(equal, Int)
        auto other = Int::cast(args[0]);
        if (other)
            return New(Bool(self->value == other->value));
        return New(Bool(false));
    }

    $method(not_equal, Int)
        auto other = Int::cast(args[0]);
        if (other)
            return New(Bool(self->value != other->value));
        return New(Bool(false));
    }

    $method(greater, Int)
        return New(Int(self->value > Int::getValue(args[0])));
    }

    $method(less, Int)
        return New(Int(self->value < Int::getValue(args[0])));
    }

    $method(greater_or_equal, Int)
        return New(Int(self->value >= Int::getValue(args[0])));
    }

    $method(less_or_equal, Int)
        return New(Int(self->value <= Int::getValue(args[0])));
    }

    static void populate() {
//        TODO: populate
        addMethod("init", init, 1);
        addMethod("str", str, 0);
        addMethod("add", add, 1);
        addMethod("sub", subtract, 1);
        addMethod("mul", multiply, 1);
        addMethod("div", divide, 1);

        addMethod("eq", equal, 1);
        addMethod("leq", less_or_equal, 1);
    }
};

#endif //INTERPRETER_INT_H
