#include "Callable.h"

#include "Types/Array.h"
#include "../Parser/Statement/Statement.h"
#include "Class.h"

Callable::Callable() {
    closure = std::make_shared<Scope>();
}

Callable::Callable(Scope::ptr closure) : closure(closure) {
    assert(closure);
}

FunctionPrototype::FunctionPrototype(std::vector<Argument> &arguments) : Callable(), arguments(arguments) {
    required = 0;
    for (auto &&argument : arguments) {
        if (argument.defaultValue == nullptr and not argument.variable)
            required++;
        if (argument.variable)
            variable = true;
    }
}

void FunctionPrototype::checkArguments(ArgsList positional, KwargsList keyword) {
    auto count = positional.size() + keyword.size();
    if ((count > arguments.size() and not variable) or count < required) {
        std::string message = "This function takes ";
//        TODO: add `at least`
        if (arguments.size() != required)
            message += "from " + std::to_string(required) + " to ";
        throw Interpreter::ExceptionWrapper(new ValueError(
                message + std::to_string(arguments.size()) + " arguments, but " + std::to_string(count) + " provided")
        );
    }
    for (auto i = 0; i < positional.size(); ++i) {
        if (keyword.count(arguments[i].name) > 0)
            throw Interpreter::ExceptionWrapper(new ValueError("Duplicate argument " + arguments[i].name));
        if (not arguments[i].positional)
            throw Interpreter::ExceptionWrapper(new ValueError("Argument " + arguments[i].name + " is keyword-only"));
        if (arguments[i].variable)
            break;
    }
    for (auto i = positional.size(); i < required; ++i)
        if (keyword.count(arguments[i].name) == 0)
            throw Interpreter::ExceptionWrapper(new ValueError("Argument " + arguments[i].name + " is not set"));
}

Function::Function(Statement *body, Scope::ptr closure, std::vector<FunctionPrototype::Argument> &arguments) :
        FunctionPrototype(arguments), body(body) {
    assert(closure);
    this->closure = closure;
}

ObjPtr Function::call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) {
    long size = positional.size();
    for (int i = 0; i < size; ++i) {
        if (arguments[i].variable) {
            auto args = New(Array(std::vector<ObjPtr>(positional.begin() + i, positional.end())));
            interpreter->defineVariable(arguments[i].name, args);
            break;
        } else
            interpreter->defineVariable(arguments[i].name, positional[i]);
    }

    if (arguments.size() > size and arguments[size].variable) {
        interpreter->defineVariable(arguments[size].name, New(Array()));
        size++;
    }

    for (int i = size; i < arguments.size(); ++i) {
        auto kw = keyword.find(arguments[i].name);
        if (kw != keyword.end())
            interpreter->defineVariable(arguments[i].name, kw->second);
        else {
            assert(arguments[i].defaultValue != nullptr);
            interpreter->defineVariable(arguments[i].name, arguments[i].defaultValue);
        }
    }
    body->visit(interpreter);
    return nullptr;
}

void ClassMethod::checkArguments(ArgsList positional, KwargsList keyword) {
    return function->checkArguments(positional, keyword);
}

ObjPtr ClassMethod::call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) {
    return function->call(interpreter, positional, keyword);
}

ClassMethod::ClassMethod(Callable::ptr function, Instance::ptr instance) :
        Callable(), function(function), instance(instance) {
    if (function->closure)
        closure = std::make_shared<Scope>(function->closure);
    closure->defineVariable("this", instance);
}
