#ifndef INTERPRETER_SCOPE_H
#define INTERPRETER_SCOPE_H

#include <utility>
#include <cassert>
#include "Object.h"

class Scope : public Object {
public:
    ObjPtr findAttribute(const std::string &name, long level) {
        assert(level >= 0);
        auto result = Object::findAttribute(name);
        if (level == 0)
            return result;
        assert(upper);
        return upper->findAttribute(name, level - 1);
    }

    void setAttribute(const std::string &name, ObjPtr value, long level) {
        assert(level >= 0);
        if (level == 0) {
            assert(attributes.count(name) != 0);
            attributes[name] = value;
            return;
        }
        if (attributes.find(name) != attributes.end())
            attributes[name] = value;
        assert(upper);
        upper->setAttribute(name, value, level - 1);
    }

    void defineVariable(const std::string &name, ObjPtr value) {
        attributes[name] = value;
    }

    typedef std::shared_ptr<Scope> ptr;

    ptr getUpper() {
        assert(upper);
        return upper;
    }

    void setUpper(ptr scope) {
        assert(!upper);
        upper = std::move(scope);
    }

    bool hasUpper() {
        return bool(upper);
    }

private:
    friend class Class;
    ptr upper{nullptr};
};

#endif //INTERPRETER_SCOPE_H
