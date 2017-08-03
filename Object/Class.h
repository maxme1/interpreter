#ifndef INTERPRETER_CLASS_H
#define INTERPRETER_CLASS_H

#include "Types/Callable.h"
#include "Types/Exception.h"
#include "Types/Scope.h"
#include "Types/None.h"


class ClassInstance : public Object {
    Object *classPtr;

public:
    ClassInstance(Object *classPtr) {
        this->classPtr = classPtr;
        classPtr->save();
    }

    ~ClassInstance() override {
        if (classPtr->canDelete())
            delete classPtr;
    }

    Object *findAttribute(const std::string &name) override {
        auto result = Object::findAttribute(name);
        if (!result)
            result = classPtr->findAttribute(name);
        if(!result)
            return nullptr;
//        creating a class method
        auto method = dynamic_cast<Callable *> (result);
        if (method)
            return new ClassMethod(this, method);
        return result;
    }
};

class Class : public Callable {
    friend class Interpreter;
protected:

    std::string argument(int i) override {
        return nullptr;
    }

    bool checkArguments(int count) override { return count == 0; }

    Object *__call__(Object *args, Interpreter *interpreter) override {
        return new ClassInstance(this);
    }

public:
    explicit Class(Scope *context) : Callable(context) {
//        copy the class's scope
        attributes = context->attributes;
        for (auto &&attribute : attributes) {
            attribute.second->save();
        }
    }
};

#endif //INTERPRETER_CLASS_H
