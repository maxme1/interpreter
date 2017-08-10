#ifndef INTERPRETER_CLASS_H
#define INTERPRETER_CLASS_H

#include "Callable.h"

class Class;
class Instance : public Object {
protected:
    std::shared_ptr<Class> classPtr;
public:
    explicit Instance(std::shared_ptr<Class> classPtr);

    std::string asString() override;
    ObjPtr findAttribute(const std::string &name) override;

    virtual std::shared_ptr<Class> getClass();

    typedef std::shared_ptr<Instance> ptr;
};

class Class : public Object {
public:
    typedef std::shared_ptr<Class> ptr;

    explicit Class(Class::ptr superclass);
    Class(const std::string &name, Scope::ptr body, Class::ptr superclass, Scope::ptr context);
    std::string asString() override;
    ObjPtr findAttribute(const std::string &name) override;

    Class::ptr getSuperClass();

private:
    friend class Interpreter;
    ptr superclass{nullptr};
protected:
    virtual Instance::ptr makeInstance(const ptr &instanceClass);
};

#endif //INTERPRETER_CLASS_H
