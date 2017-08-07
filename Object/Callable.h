#ifndef INTERPRETER_FUNCTION_H
#define INTERPRETER_FUNCTION_H


#include <utility>

#include "Object.h"
#include "../Parser/Statement/Statement.h"
#include "../Interpreter/API.h"

typedef const std::vector<Object *> &ArgsList;

class Callable : public Object {
    friend class Interpreter;
    friend class ClassMethod;
    Scope *context = nullptr;
protected:
    virtual bool checkArguments(int count) = 0;
    virtual Object *__call__(ArgsList args, API *api) = 0;
public:
    Callable() = default;
    explicit Callable(Scope *context);
    ~Callable() override;
};

class Function : public Callable {
    friend class Interpreter;
    Statement *body = nullptr;
    std::vector<std::string> arguments;
    bool unlimited;

protected:
    bool checkArguments(int count) override;
    Object *__call__(ArgsList args, API *api) override;

public:
    explicit Function(std::vector<std::string> arguments, Statement *body, bool unlimited, Scope *context);
    ~Function() override;
};

class ClassMethod : public Callable {
    friend class Interpreter;
    Callable *function;
    Object *source;
protected:
    bool checkArguments(int count) override;
    Object *__call__(ArgsList args, API *api) override;

public:
    ClassMethod(Object *source, Callable *function);
    ~ClassMethod() override;
};

#endif //INTERPRETER_FUNCTION_H
