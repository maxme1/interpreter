#ifndef INTERPRETER_INT_H
#define INTERPRETER_INT_H

#include "../Exception.h"
#include "../Native/Native.h"
#include "String.h"
#include "Bool.h"

$class(Int)
    int value;
    Int() = default;

    explicit Int(int value) : value(value) {}

    static int getValue(ObjPtr object) {
        return cast(object, true)->value;
    }

    bool asBool() override {
        return value != 0;
    }

    std::string asString() override {
        return std::to_string(value);
    }

    $method(init, Int)
        if (cast(positional[0]))
            self->value = Int::getValue(positional[0]);
        else if (String::cast(positional[0]))
            self->value = std::stoi(positional[0]->asString());
        else if (Bool::cast(positional[0]))
            self->value = (int) positional[0]->asBool();
        else
            assert(false);
//            throw Wrap(new ValueError("Could not convert to Int"));
        return nullptr;
    }

    $method(str, Int)
        return New(String(self->asString()));
    }

    $method(plus, Int)
        return New(Int(self->value + Int::getValue(positional[0])));
    }

    $method(divide, Int)
        int val = Int::getValue(positional[0]);
        assert (val != 0);
//            throw Wrap(new Exception("Division by zero"));
        return New(Int(self->value / val));
    }

    $method(unary_add, Int)
        return New(Int(self->value));
    }

    $method(unary_subtract, Int)
        return New(Int(-self->value));
    }

    $method(multiply, Int)
        return New(Int(self->value * Int::getValue(positional[0])));
    }

    $method(subtract, Int)
        return New(Int(self->value - Int::getValue(positional[0])));
    }

    $method(equal, Int)
        auto other = Int::cast(positional[0]);
        if (other)
            return New(Bool(self->value == other->value));
        return New(Bool(false));
    }

    $method(not_equal, Int)
        auto other = Int::cast(positional[0]);
        if (other)
            return New(Bool(self->value != other->value));
        return New(Bool(false));
    }

    $method(greater, Int)
        return New(Int(self->value > Int::getValue(positional[0])));
    }

    $method(less, Int)
        return New(Int(self->value < Int::getValue(positional[0])));
    }

    $method(greater_or_equal, Int)
        return New(Int(self->value >= Int::getValue(positional[0])));
    }

    $method(less_or_equal, Int)
        return New(Int(self->value <= Int::getValue(positional[0])));
    }

    static void populate() {
        addMethod("init", init, 1);
        addMethod("str", str, 0);

        addMethod("plus", plus, 1);
        addMethod("minus", subtract, 1);
        addMethod("plusUnary", unary_add, 1);
        addMethod("minusUnary", unary_subtract, 1);
        addMethod("multiply", multiply, 1);
        addMethod("divide", divide, 1);
        addMethod("greater", greater, 1);
        addMethod("less", less, 1);
        addMethod("lessEqual", less_or_equal, 1);
        addMethod("greaterEqual", greater_or_equal, 1);
        addMethod("equal", equal, 1);
        addMethod("not_equal", not_equal, 1);
    }
};

#endif //INTERPRETER_INT_H
