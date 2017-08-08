#ifndef INTERPRETER_CLASS_H
#define INTERPRETER_CLASS_H

#include "Callable.h"
#include "Types/Exception.h"
#include "Types/None.h"

class Class;
class Instance : public Object {
    Class *classPtr;
protected:

public:
    explicit Instance(Class *classPtr);
    ~Instance() override;

    Object *findAttribute(const std::string &name) override;
    Object *getSuperClass();
};

class Class : public Callable {
    friend class Interpreter;
    friend class Instance;
    Object *superclass;
protected:
    bool checkArguments(int count) override;
    Object *__call__(ArgsList args, API *api) override;

public:
    Class() = default;
    Class(Object *context, Class *superclass);
    virtual ~Class();

    Object *findAttribute(const std::string &name) override;
};

#endif //INTERPRETER_CLASS_H
