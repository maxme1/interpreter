#ifndef INTERPRETER_INT_H
#define INTERPRETER_INT_H

#include "../Object.h"
#include "Bool.h"
#include "Exception.h"

class Int : public Object {
    Int *getInt(Object *target) {
        Int *local = dynamic_cast<Int *>(target);
        if (local) {
            return local;
        }
        throw Exception("Operator undefined for these types");
    }

public:
    bool asBool() override {
        return value != 0;
    }

    int value;

    explicit Int(int value) : value(value) {}

    std::string str() {
        return std::to_string(value);
    }

    Object *add(Object *other) { return new Int(value + getInt(other)->value); }

    Object *unary_add() { return this; }

    Object *unary_subtract() { return new Int(-value); }

    Object *subtract(Object *other) { return new Int(value - getInt(other)->value); }

    Object *multiply(Object *other) { return new Int(value * getInt(other)->value); }

    Object *divide(Object *other) {
        int val = getInt(other)->value;
        if (val == 0)
            throw Exception("Division by zero");
        return new Int(value / val);
    }

    Object *equal(Object *other) { return new Int(value == getInt(other)->value); }

    Object *greater(Object *other) { return new Int(value > getInt(other)->value); }

    Object *less(Object *other) { return new Int(value < getInt(other)->value); }

    Object *greater_or_equal(Object *other) { return new Int(value >= getInt(other)->value); }

    Object *less_or_equal(Object *other) { return new Int(value <= getInt(other)->value); }

    Object *not_equal(Object *other) { return new Int(value != getInt(other)->value); }
};


#endif //INTERPRETER_INT_H
