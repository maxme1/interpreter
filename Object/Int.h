#ifndef INTERPRETER_INT_H
#define INTERPRETER_INT_H

#include "Object.h"
#include "Bool.h"

class Int : public Object {
public:
    int value;

    Int(int value) : value(value) {}

    std::string str() {
        return std::to_string(value);
    }

    Object *add(Object *other) {
        Int *local = dynamic_cast<Int *>(other);
        if (local != NULL) {
            return new Int(value + local->value);
        }
//    TODO: also raise
        return nullptr;
    }

    Object *uadd() {
        return this;
    }

    Object *usub() {
        return new Int(-value);
    }

    Object *sub(Object *other) {
        Int *local = dynamic_cast<Int *>(other);
        if (local != NULL) {
            return new Int(value - local->value);
        }
//    TODO: also raise
        return nullptr;
    }

    Object *mul(Object *other) {
        Int *local = dynamic_cast<Int *>(other);
        if (local != NULL) {
            return new Int(value * local->value);
        }
        return nullptr;
    }

    Object *div(Object *other) {
        Int *local = dynamic_cast<Int *>(other);
        if (local != NULL) {
            return new Int(value / local->value);
        }
        return nullptr;
    }

    Bool *__bool__() override {
        return new Bool(value != 0);
    }
};


#endif //INTERPRETER_INT_H
