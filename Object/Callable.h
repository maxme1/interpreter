#ifndef INTERPRETER_FUNCTION_H
#define INTERPRETER_FUNCTION_H

#include <utility>

#include "Object.h"
#include "../TreeWalk/API.h"
#include "Scope.h"

typedef const std::vector<ObjPtr> &ArgsList;

class Interpreter;
struct Callable : public Object {
    friend class Interpreter;
    friend class ClassMethod;
    Scope::ptr closure = nullptr;
protected:
    virtual bool checkArguments(int count) = 0;
    virtual ObjPtr call(ArgsList args, Interpreter *interpreter) = 0;
public:
    Callable() = default;
    explicit Callable(Scope::ptr closure);

    typedef std::shared_ptr<Callable> ptr;
};

class Statement;
class Function : public Callable {
    friend class Interpreter;
    Statement *body = nullptr;
    std::vector<std::string> arguments;
    bool unlimited;

protected:
    bool checkArguments(int count) override;
    ObjPtr call(ArgsList args, Interpreter *interpreter) override;

public:
    explicit Function(std::vector<std::string> &arguments, Statement *body, bool unlimited, Scope::ptr closure);
};

class Instance;
class ClassMethod : public Callable {
    friend class Interpreter;
    Callable::ptr function;
    std::shared_ptr<Instance> instance;
protected:
    bool checkArguments(int count) override;
    ObjPtr call(ArgsList args, Interpreter *interpreter) override;

public:
    ClassMethod(Callable::ptr function, std::shared_ptr<Instance> instance);
};

#endif //INTERPRETER_FUNCTION_H
