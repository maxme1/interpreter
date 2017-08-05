#ifndef INTERPRETER_FUNCTION_H
#define INTERPRETER_FUNCTION_H


#include "Object.h"
#include "../Parser/Statement/Statement.h"

typedef const std::vector<Object *> &ArgsList;

class Callable : public Object {
    friend class Interpreter;
    friend class ClassMethod;
    Scope *context = nullptr;
protected:
    virtual bool checkArguments(int count) = 0;
    virtual Object *__call__(ArgsList args, Interpreter *interpreter) = 0;
public:
    Callable() = default;
    explicit Callable(Scope *context);
    ~Callable() override;
};

class Function : public Callable {
    friend class Interpreter;
    Statement *body = nullptr;
    std::vector<std::string> arguments;

protected:
    bool checkArguments(int count) override;
    Object *__call__(ArgsList args, Interpreter *interpreter) override;

public:
    explicit Function(const std::vector<std::string> &arguments, Statement *body, Scope *context) :
            Callable(context), body(body), arguments(arguments) {}

    ~Function() override;
};

class ClassMethod : public Callable {
    friend class Interpreter;
    Callable *function;
    Object *source;
protected:
    bool checkArguments(int count) override;
    Object *__call__(ArgsList args, Interpreter *interpreter) override;

public:
    ClassMethod(Object *source, Callable *function);
    ~ClassMethod() override;
};

#endif //INTERPRETER_FUNCTION_H
