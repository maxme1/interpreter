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

Function::Function(Statement *body, Scope::ptr closure, std::vector<std::string> &arguments,
                   std::map<std::string, ObjPtr> defaults) :
        Callable(std::move(closure)), body(body), arguments(arguments), defaults(defaults) {}

bool Function::checkArguments(ArgsList positional, KwargsList keyword) {
    auto count = positional.size();
    auto result = count <= arguments.size() and count >= arguments.size() - defaults.size();
//    TODO: optimize
    if (not result)
        throw Interpreter::ExceptionWrapper(new ValueError("bad args count"));
    return result;
//    auto size = arguments.size();
//    if (unlimited)
//        size--;
//    return size == count or (arguments.size() <= count and unlimited);
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

bool ClassMethod::checkArguments(ArgsList positional, KwargsList keyword) {
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
