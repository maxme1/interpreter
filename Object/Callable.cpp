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

FunctionPrototype::FunctionPrototype(std::vector<std::string> &arguments, std::map<std::string, ObjPtr> defaults) :
        Callable(), arguments(arguments), defaults(defaults) {}

void FunctionPrototype::checkArguments(ArgsList positional, KwargsList keyword) {
    auto count = positional.size() + keyword.size(), required = arguments.size() - defaults.size();
    if (not(count <= arguments.size() and count >= required)) {
        std::string message = "This function takes ";
        if (arguments.size() != required)
            message += "from " + std::to_string(required) + " to ";
        throw Interpreter::ExceptionWrapper(new ValueError(
                message + std::to_string(arguments.size()) + " arguments, but " + std::to_string(count) + " provided")
        );
    }
    for (auto i = 0; i < positional.size(); ++i)
        if (keyword.count(arguments[i]) > 0)
            throw Interpreter::ExceptionWrapper(new ValueError("Duplicate argument " + arguments[i]));
    for (auto i = positional.size(); i < required; ++i)
        if (keyword.count(arguments[i]) == 0)
            throw Interpreter::ExceptionWrapper(new ValueError("Argument " + arguments[i] + " is not set"));
}

Function::Function(Statement *body, Scope::ptr closure, std::vector<std::string> &arguments,
                   std::map<std::string, ObjPtr> defaults) :
        FunctionPrototype(arguments, defaults), body(body) {
    assert(closure);
    this->closure = closure;
}

ObjPtr Function::call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) {
    long size = positional.size(), i;
    for (i = 0; i < size; ++i)
        interpreter->defineVariable(arguments[i], positional[i]);

    for (; i < arguments.size(); ++i) {
//        TODO: optimize
        if (keyword.count(arguments[i]) > 0)
            interpreter->defineVariable(arguments[i], keyword.at(arguments[i]));
        else
            interpreter->defineVariable(arguments[i], defaults[arguments[i]]);
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
