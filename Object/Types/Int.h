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

    static int toInt(ObjPtr object) {
        return cast(object, true)->value;
    }

    bool asBool() override {
        return value != 0;
    }

    $method(init, Int)
        if (cast(positional[0]))
            self->value = Int::toInt(positional[0]);
        else if (String::cast(positional[0]))
            self->value = std::stoi(String::toString(positional[0], interpreter));
        else if (Bool::cast(positional[0]))
            self->value = (int) positional[0]->asBool();
        else
            throw Interpreter::ExceptionWrapper(new ValueError("Could not convert object to Int"));
        return nullptr;
    }

    $method(str, Int)
        return New(String(std::to_string(self->value)));
    }

    $method(plus, Int)
        return New(Int(self->value + Int::toInt(positional[0])));
    }

    $method(divide, Int)
        int val = Int::toInt(positional[0]);
        if (val == 0)
            throw Interpreter::ExceptionWrapper(new ArithmeticError("Division by zero"));
        return New(Int(self->value / val));
    }

    $method(unary_add, Int)
        return New(Int(self->value));
    }

    $method(unary_subtract, Int)
        return New(Int(-self->value));
    }

    $method(multiply, Int)
        return New(Int(self->value * Int::toInt(positional[0])));
    }

    $method(subtract, Int)
        return New(Int(self->value - Int::toInt(positional[0])));
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
        return New(Bool(true));
    }

    $method(greater, Int)
        return New(Bool(self->value > Int::toInt(positional[0])));
    }

    $method(less, Int)
        return New(Bool(self->value < Int::toInt(positional[0])));
    }

    $method(greater_or_equal, Int)
        return New(Bool(self->value >= Int::toInt(positional[0])));
    }

    $method(less_or_equal, Int)
        return New(Bool(self->value <= Int::toInt(positional[0])));
    }

    static void populate() {
        addMethod("init", init, 1);
        addMethod("str", str, 0);

        addMethod("plus", plus, 1);
        addMethod("minus", subtract, 1);
        addMethod("unaryPlus", unary_add, 0);
        addMethod("unaryMinus", unary_subtract, 0);
        addMethod("multiply", multiply, 1);
        addMethod("divide", divide, 1);
        addMethod("greater", greater, 1);
        addMethod("less", less, 1);
        addMethod("lessEqual", less_or_equal, 1);
        addMethod("greaterEqual", greater_or_equal, 1);
        addMethod("equal", equal, 1);
        addMethod("notEqual", not_equal, 1);
    }
};

#endif //INTERPRETER_INT_H
