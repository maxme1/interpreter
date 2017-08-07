#ifndef INTERPRETER_SCOPE_H
#define INTERPRETER_SCOPE_H

#include "../Object.h"
#include "Exception.h"

class Scope : public Object {
    friend class Class;
    Scope *upper = nullptr;
public:
    Scope *getUpper() { return upper; }

    void setUpper(Scope *upper) {
        this->upper = upper;
        this->upper->save();
    }

    Object *findAttribute(const std::string &name) override {
        auto result = Object::findAttribute(name);
        if (result)
            return result;
//        avoid cycles
        if (upper and upper != this)
            return upper->findAttribute(name);
        return nullptr;
    }

    ~Scope() override {
        if (upper and upper->canDelete())
            delete upper;
    }
};


#endif //INTERPRETER_SCOPE_H
