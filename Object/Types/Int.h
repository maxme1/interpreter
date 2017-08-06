#ifndef INTERPRETER_INT_H
#define INTERPRETER_INT_H

#include "../Object.h"
#include "Bool.h"
#include "Exception.h"
#include "../Native/NativeObject.h"

struct Int : public BaseNative {
    int value;

    explicit Int(int value) : value(value) {}

    static int getInt(Object *target) {
        Int *local = dynamic_cast<Int *>(target);
        if (local) {
            return local->value;
        }
        throw Exception("Operator undefined for these types");
    }

    bool asBool() override {
        return value != 0;
    }

    std::string asString() override {
        return std::to_string(value);
    }

    Object *add(Object *other) override { return new Int(value + getInt(other)); }

    Object *unary_add() override { return this; }

    Object *unary_subtract() override { return new Int(-value); }

    Object *subtract(Object *other) override { return new Int(value - getInt(other)); }

    Object *multiply(Object *other) override { return new Int(value * getInt(other)); }

    Object *divide(Object *other) override {
        int val = getInt(other);
        if (val == 0)
            throw Exception("Division by zero");
        return new Int(value / val);
    }

    Object *equal(Object *other) override { return new Bool(value == getInt(other)); }

    Object *greater(Object *other) override { return new Bool(value > getInt(other)); }

    Object *less(Object *other) override { return new Bool(value < getInt(other)); }

    Object *greater_or_equal(Object *other) override { return new Bool(value >= getInt(other)); }

    Object *less_or_equal(Object *other) override { return new Bool(value <= getInt(other)); }

    Object *not_equal(Object *other) override { return new Bool(value != getInt(other)); }
};


#endif //INTERPRETER_INT_H
