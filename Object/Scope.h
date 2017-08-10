#ifndef INTERPRETER_SCOPE_H
#define INTERPRETER_SCOPE_H

#include <utility>
#include "Object.h"

class Scope : public Object {
public:
    ObjPtr findAttribute(const std::string &name) override {
        auto result = Object::findAttribute(name);
        if (!result and upper)
            return upper->findAttribute(name);
        return result;
    }

    typedef std::shared_ptr<Scope> ptr;

    void setUpper(Scope::ptr scope) {
        if (!upper)
            upper = std::move(scope);
    }

private:
    friend class Class;
    ptr upper{nullptr};
};

#endif //INTERPRETER_SCOPE_H
