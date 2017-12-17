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

Function::Function(std::vector<std::string> &arguments, Statement *body, bool unlimited, Scope::ptr closure) :
        Callable(std::move(closure)), body(body), arguments(arguments), unlimited(unlimited) {}

bool Function::checkArguments(int count) {
    return arguments.size() == count;
//    auto size = arguments.size();
//    if (unlimited)
//        size--;
//    return size == count or (arguments.size() <= count and unlimited);
}

ObjPtr Function::call(ArgsList args, Interpreter *interpreter) {
//        populating with arguments
    long size = arguments.size(), i;
//    if (unlimited)
//        size--;
    for (i = 0; i < size; ++i)
        interpreter->defineVariable(arguments[i], args[i]);
//    if (unlimited) {
//        auto last = std::vector<ObjPtr>(args.begin() + size, args.end());
//        interpreter->setVariable(arguments[size], New(Array(last)));
//    }
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
