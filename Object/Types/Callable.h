#ifndef INTERPRETER_FUNCTION_H
#define INTERPRETER_FUNCTION_H


#include "../Object.h"

class Callable : public Object {
    friend class Interpreter;
    friend class ClassMethod;
protected:
    virtual bool checkArguments(int count) = 0;
    virtual std::string argument(int i) = 0;
    virtual Object *__call__(Object *args, Interpreter *interpreter) = 0;
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
    explicit Function(const std::vector<std::string> &arguments, Statement *body) :
            body(body), arguments(arguments) {}

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
    ClassMethod(Object *source, Callable *function) {
        this->function = function;
        this->source = source;
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
