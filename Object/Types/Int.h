#ifndef INTERPRETER_INT_H
#define INTERPRETER_INT_H

#include "../Object.h"
#include "Bool.h"
#include "Exception.h"
#include "../Native.h"

//struct A : public NativeClass {
//    int x = 0;
//
//    A() {
//        methods["test"] = {test, {}};
//    }
//
//    static Object *test(NativeClass *_self, Object *args) {
//        cast(A);
//        std::cout << self->x;
//        return nullptr;
//    }
//};

struct Int : public NativeClass {
    int getInt(Object *target) {
        Int *local = dynamic_cast<Int *>(target);
        if (local) {
            return local->value;
        }
        throw Exception("Operator undefined for these types");
    }

public:
    bool asBool() override {
        return value != 0;
    }

    int value;

    explicit Int(int value) : value(value) {
        methods["add"] = {add, {"other"}};
    }

    std::string str() {
        return std::to_string(value);
    }

//    Object *add(Object *other) { return new Int(value + getInt(other)->value); }

    static Object *add(NativeClass *_self, ArgsList args) {
        cast(Int);
        return new Int(self->value + self->getInt(args[0]));
    }

    Object *unary_add() { return this; }

    Object *unary_subtract() { return new Int(-value); }

    Object *subtract(Object *other) { return new Int(value - getInt(other)); }

    Object *multiply(Object *other) { return new Int(value * getInt(other)); }

    Object *divide(Object *other) {
        int val = getInt(other);
        if (val == 0)
            throw Exception("Division by zero");
        return new Int(value / val);
    }

    Object *equal(Object *other) { return new Int(value == getInt(other)); }

    Object *greater(Object *other) { return new Int(value > getInt(other)); }

    Object *less(Object *other) { return new Int(value < getInt(other)); }

    Object *greater_or_equal(Object *other) { return new Int(value >= getInt(other)); }

    Object *less_or_equal(Object *other) { return new Int(value <= getInt(other)); }

    Object *not_equal(Object *other) { return new Int(value != getInt(other)); }
};


#endif //INTERPRETER_INT_H
