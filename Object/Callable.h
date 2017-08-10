#ifndef INTERPRETER_FUNCTION_H
#define INTERPRETER_FUNCTION_H


#include <utility>

#include "Object.h"
#include "../Interpreter/API.h"
#include "Scope.h"

typedef const std::vector<ObjPtr> &ArgsList;

class Callable : public Object {
    friend class Interpreter;
    friend class ClassMethod;
    Scope::ptr context = nullptr;
protected:
    virtual bool checkArguments(int count) = 0;
    virtual ObjPtr call(ArgsList args, API *api) = 0;
public:
    Callable() = default;
    explicit Callable(Scope::ptr context);

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
    ObjPtr call(ArgsList args, API *api) override;

public:
    explicit Function(std::vector<std::string> &arguments, Statement *body, bool unlimited, Scope::ptr context);
};

// TODO: subclass
class Instance;
class ClassMethod : public Callable {
    friend class Interpreter;
    Callable::ptr function;
    std::shared_ptr<Instance> instance;
protected:
    bool checkArguments(int count) override;
    ObjPtr call(ArgsList args, API *api) override;

public:
    ClassMethod(Callable::ptr function, std::shared_ptr<Instance> instance);
};

#endif //INTERPRETER_FUNCTION_H
