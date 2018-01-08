#ifndef INTERPRETER_CLASS_H
#define INTERPRETER_CLASS_H

#include "Callable.h"

class Class : public Callable {
    friend class Interpreter;
private:
    shared(Class) superClass;
    std::string name;
public:
    typedef std::shared_ptr<Class> ptr;

    Class(const std::string &name, Scope::ptr body, Class::ptr superclass, Scope::ptr closure);
    Class(const std::string &name, Class::ptr superclass);
    ObjPtr findAttribute(const std::string &name) override;
    std::string getName();
    std::string asString() override;
protected:
    virtual ObjPtr makeInstance(ptr base);
    void checkArguments(ArgsList positional, KwargsList keyword) override;
    ObjPtr call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) override;
};

class Instance : public Object {
protected:
    shared(Class) theClass;
public:
    explicit Instance(shared(Class) theClass);

    ObjPtr findAttribute(const std::string &name) override;

    virtual shared(Class) getClass();
    typedef shared(Instance) ptr;
    std::string asString() override;
};

#endif //INTERPRETER_CLASS_H
