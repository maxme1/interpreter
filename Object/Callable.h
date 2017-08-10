#ifndef INTERPRETER_FUNCTION_H
#define INTERPRETER_FUNCTION_H


#include <utility>

#include "Object.h"
#include "../Interpreter/API.h"
#include "Scope.h"

typedef const std::vector<Object *> &ArgsList;

class Callable : public Object {
    friend class Interpreter;
    friend class ClassMethod;
    Scope *context = nullptr;
protected:
    virtual bool checkArguments(int count) = 0;
    virtual Object *call(ArgsList args, API *api) = 0;
public:
    Callable() = default;
    explicit Callable(Scope *context);
    ~Callable() override;
};

class Statement;
class Function : public Callable {
    friend class Interpreter;
    Statement *body = nullptr;
    std::vector<std::string> arguments;
    bool unlimited;

protected:
    bool checkArguments(int count) override;
    Object *call(ArgsList args, API *api) override;

public:
    explicit Function(std::vector<std::string> arguments, Statement *body, bool unlimited, Scope *context);
    ~Function() override;
};

// TODO: subclass
class Instance;
class ClassMethod : public Callable {
    friend class Interpreter;
    Callable *function;
    Instance *instance;
protected:
    bool checkArguments(int count) override;
    Object *call(ArgsList args, API *api) override;

public:
    ClassMethod(Callable *function, Instance *instance);
    ~ClassMethod() override;
};

#endif //INTERPRETER_FUNCTION_H
