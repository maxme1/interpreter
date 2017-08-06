#ifndef INTERPRETER_CLASS_H
#define INTERPRETER_CLASS_H

#include "Callable.h"
#include "Types/Exception.h"
#include "Types/Scope.h"
#include "Types/None.h"


class Instance : public Object {
protected:
    Object *classPtr;

public:
    Instance() = default;
    explicit Instance(Object *classPtr);
    ~Instance() override;

    Object *findAttribute(const std::string &name) override;
};

class Class : public Callable {
    friend class Interpreter;
protected:
    bool checkArguments(int count) override;
    Object *__call__(ArgsList args, Interpreter *interpreter) override;

public:
    Class() = default;
    explicit Class(Scope *context);
};

#endif //INTERPRETER_CLASS_H
