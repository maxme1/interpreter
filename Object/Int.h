#ifndef INTERPRETER_INT_H
#define INTERPRETER_INT_H

#include "Object.h"
#include "Bool.h"

class Int : public Object {
public:
    bool asBool() override {
        return value != 0;
    }

    int value;

    explicit Int(int value) : value(value) {}

    std::string str() {
        return std::to_string(value);
    }

    Object *add(Object *other) {
        Int *local = dynamic_cast<Int *>(other);
        if (local) {
            return new Int(value + local->value);
        }
//        TODO: add types
        throw "Operator undefined for these types";
    }

    Object *uadd() {
        return this;
    }

    Object *usub() {
        return new Int(-value);
    }

    Object *sub(Object *other) {
        Int *local = dynamic_cast<Int *>(other);
        if (local) {
            return new Int(value - local->value);
        }
        throw "Operator undefined for these types";
    }

    Object *mul(Object *other) {
        Int *local = dynamic_cast<Int *>(other);
        if (local) {
            return new Int(value * local->value);
        }
        throw "Operator undefined for these types";
    }

    Object *div(Object *other) {
        Int *local = dynamic_cast<Int *>(other);
        if (local) {
            return new Int(value / local->value);
        }
        throw "Operator undefined for these types";
    }
};


#endif //INTERPRETER_INT_H
