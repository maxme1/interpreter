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

bool Function::checkArguments(int count) {
    return count <= arguments.size() and count >= arguments.size() - defaults.size();
//    auto size = arguments.size();
//    if (unlimited)
//        size--;
//    return size == count or (arguments.size() <= count and unlimited);
}

ObjPtr Function::call(ArgsList args, Interpreter *interpreter) {
    long size = args.size(), i;
    for (i = 0; i < size; ++i)
        interpreter->defineVariable(arguments[i], args[i]);

    for (; i < arguments.size(); ++i)
//        TODO: optimize
        interpreter->defineVariable(arguments[i], defaults[arguments[i]]);
    body->visit(interpreter);
    return nullptr;
}

bool ClassMethod::checkArguments(int count) {
    return function->checkArguments(count);
}

ObjPtr ClassMethod::call(ArgsList args, Interpreter *interpreter) {
    return function->call(args, interpreter);
}

ClassMethod::ClassMethod(Callable::ptr function, Instance::ptr instance) :
        Callable(), function(function), instance(instance) {
    if (function->closure)
        closure = std::make_shared<Scope>(function->closure);
    closure->defineVariable("this", instance);
}
