#ifndef INTERPRETER_CLASS_H
#define INTERPRETER_CLASS_H

#include "Types/Callable.h"
#include "Types/Exception.h"


class ClassInstance : public Object {
    Object *classPtr;

public:
    ClassInstance(Object *classPtr) {
        this->classPtr = classPtr;
        classPtr->save();
    }

    Object *getAttribute(const std::string &name) override {
        auto value = attributes.find(name);
        Object *result;
        if (value != attributes.end())
            result = value->second;
        else
            result = classPtr->getAttribute(name);
        auto method = dynamic_cast<Callable *> (result);
        if (method)
            return new ClassMethod(this, method);
        return result;
    }
};

class Class : public Callable {
protected:

    std::string argument(int i) override {
        return nullptr;
    }

    bool checkArguments(int count) override { return true; }

    Object *__call__(Object *args, Interpreter *interpreter) override {
        return new ClassInstance(this);
//      TODO:  call init
    }

public:
    explicit Class(Object *scope) {
//        copy the class's scope
        attributes = scope->attributes;
        for (auto &&attribute : attributes) {
            attribute.second->save();
        }
    }
};

#endif //INTERPRETER_CLASS_H
