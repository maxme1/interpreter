#ifndef INTERPRETER_SCOPE_H
#define INTERPRETER_SCOPE_H

#include "../Object.h"
#include "Exception.h"

class Scope : public Object {
    Scope *upper = nullptr;
public:
    Scope *getUpper() { return upper; }

    void setUpper(Scope *upper) {
        this->upper = upper;
        this->upper->save();
    }

//    TODO: unify
    Object *getAttribute(const std::string &name) override {
        auto value = attributes.find(name);
        if (value != attributes.end())
            return value->second;
        if (upper)
            return upper->getAttribute(name);
        throw Exception("No variable " + name);
    }

    ~Scope() override {
        if (upper and upper->canDelete())
            delete upper;
    }
};


#endif //INTERPRETER_SCOPE_H
