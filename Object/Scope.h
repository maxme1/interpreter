#ifndef INTERPRETER_SCOPE_H
#define INTERPRETER_SCOPE_H

#include <utility>
#include <cassert>
#include "Object.h"

class Scope {
    friend class Class;
    std::map<std::string, shared(Object) > attributes;
    shared(Scope) upper{nullptr};
public:
    typedef std::shared_ptr<Scope> ptr;

    Scope() = default;

    explicit Scope(ptr scope) {
        if (scope) {
            upper = scope->upper;
            for (auto &&attribute : scope->attributes) {
                attributes[attribute.first] = attribute.second;
            }
        }
    };

    ObjPtr findAttribute(const std::string &name, long level) {
        assert(level >= 0);
        if (level == 0) {
            auto value = attributes.find(name);
            if (value != attributes.end())
                return value->second;
            return nullptr;
        }
        assert(upper);
        return upper->findAttribute(name, level - 1);
    }

    bool setAttribute(const std::string &name, ObjPtr value, long level) {
        assert(level >= 0);
        if (level == 0) {
            if (attributes.count(name) == 0)
                return false;
            attributes[name] = value;
            return true;
        }
        assert(upper);
        upper->setAttribute(name, value, level - 1);
//        unreachable code
        assert(false);
        return false;
    }

    void defineVariable(const std::string &name, ObjPtr value) {
        attributes[name] = value;
    }

    void setUpper(ptr scope) {
        assert(!upper);
        upper = std::move(scope);
    }
};

#endif //INTERPRETER_SCOPE_H
