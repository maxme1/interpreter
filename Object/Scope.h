#ifndef INTERPRETER_SCOPE_H
#define INTERPRETER_SCOPE_H

#include "Object.h"

class Scope : public Object {
    friend class Class;
    Scope *upper{nullptr};
public:
    Scope() = default;

    ~Scope() override {
        Object::remove(upper);
    }

    Object *findAttribute(const std::string &name) override {
        auto result = Object::findAttribute(name);
        if (!result and upper)
            return upper->findAttribute(name);
        return result;
    }

    void setUpper(Scope *scope) {
        if (!upper) {
            upper = scope;
            scope->save();
        }
    }
};

#endif //INTERPRETER_SCOPE_H
