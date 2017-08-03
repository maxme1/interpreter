#ifndef INTERPRETER_FUNCTION_H
#define INTERPRETER_FUNCTION_H


#include "../Object.h"
#include "../../Parser/Statement/Statement.h"

class Callable : public Object {
    friend class Interpreter;
    friend class ClassMethod;
    Scope *context = nullptr;
protected:
    virtual bool checkArguments(int count) = 0;
    virtual std::string argument(int i) = 0;
    virtual Object *__call__(Object *args, Interpreter *interpreter) = 0;
public:
    Callable() = default;

    explicit Callable(Scope *context) : context(context) {
        context->save();
    }

    virtual ~Callable() {
        if (context and context->canDelete())
            delete context;
    }
};

class Function : public Callable {
    friend class Interpreter;
    Statement *body = nullptr;
    std::vector<std::string> arguments;

protected:
    bool checkArguments(int count) override { return arguments.size() == count; }

    std::string argument(int i) override { return arguments[i]; }

    virtual Object *__call__(Object *args, Interpreter *interpreter) {
        body->evaluate(interpreter);
    }

public:
    explicit Function(const std::vector<std::string> &arguments, Statement *body, Scope *context) :
            Callable(context), body(body), arguments(arguments) {}

    ~Function() override {
        delete body;
    }
};

class ClassMethod : public Callable {
    friend class Interpreter;
    Callable *function;
    Object *source;
protected:
    bool checkArguments(int count) override {
        return function->checkArguments(count);
    }

    std::string argument(int i) override {
        return function->argument(i);
    }

    Object *__call__(Object *args, Interpreter *interpreter) override {
        args->setAttribute("this", source);
        return function->__call__(args, interpreter);
    }

public:
    ClassMethod(Object *source, Callable *function) :
            function(function), source(source) {
        function->save();
        source->save();
    }

    virtual ~ClassMethod() {
        if (function->canDelete())
            delete function;
        if (source->canDelete())
            delete source;
    }
};

#endif //INTERPRETER_FUNCTION_H
