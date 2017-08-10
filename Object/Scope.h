#ifndef INTERPRETER_SCOPE_H
#define INTERPRETER_SCOPE_H

#include "Object.h"

class Scope : public Object {
    friend class Class;
    ObjPtr upper{nullptr};
public:
    ObjPtr findAttribute(const std::string &name) override {
        auto result = Object::findAttribute(name);
        if (!result and upper)
            return upper->findAttribute(name);
        return result;
    }

    void setUpper(Scope *scope) {
        if (!upper)
            upper = scope;
    }
};

#endif //INTERPRETER_SCOPE_H
