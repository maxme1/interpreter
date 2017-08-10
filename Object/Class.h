#ifndef INTERPRETER_CLASS_H
#define INTERPRETER_CLASS_H

#include "Callable.h"

class Class;
class Instance : public Object {
protected:
    Class *classPtr;
public:
    Instance() = default;
    explicit Instance(Class *classPtr);

    std::string asString() override;
    ObjPtr findAttribute(const std::string &name) override;

    virtual Class *getClass();
};

class Class : public Object {
    friend class Interpreter;
    Class *superclass = nullptr;
protected:
    virtual ObjPtr makeInstance(Class *instanceClass);

public:
    explicit Class(Class *superclass);
    Class(const std::string &name, Scope *body, Class *superclass, Scope *context);
    std::string asString() override;
    ObjPtr findAttribute(const std::string &name) override;

    Class *getSuperClass();
};

#endif //INTERPRETER_CLASS_H
