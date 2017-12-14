#ifndef INTERPRETER_CLASS_H
#define INTERPRETER_CLASS_H

#include "Callable.h"

struct Class : public Callable {
    friend class Interpreter;
public:
    typedef std::shared_ptr<Class> ptr;

    Class(const std::string &name, Scope::ptr body, Class::ptr superclass, Scope::ptr closure);
    std::string asString() override;
    ObjPtr findAttribute(const std::string &name) override;

    Class::ptr getSuperClass();

private:
    ptr superclass{nullptr};
protected:
    bool checkArguments(int count) override;
    ObjPtr call(const std::vector<ObjPtr> &args, Interpreter *interpreter) override;
};

class Instance : public Object {
protected:
    std::shared_ptr<Class> theClass;
public:
    explicit Instance(std::shared_ptr<Class> theClass);

    std::string asString() override;
    ObjPtr findAttribute(const std::string &name) override;

    virtual std::shared_ptr<Class> getClass();

    typedef std::shared_ptr<Instance> ptr;
};

#endif //INTERPRETER_CLASS_H
