#include "Callable.h"

#include <utility>
#include "Types/Array.h"
#include "../Parser/Statement/Statement.h"

Callable::Callable(Scope::ptr context) : context(context) {}

Function::Function(std::vector<std::string> &arguments, Statement *body, bool unlimited, Scope::ptr context) :
        Callable(context), body(body), arguments(arguments), unlimited(unlimited) {}

bool Function::checkArguments(int count) {
    int size = arguments.size();
    if (unlimited)
        size--;
    bool t = size == count or (arguments.size() <= count and unlimited);
    if (!t)
        std::cout << "expected: " << arguments.size();
    return t;
}

ObjPtr Function::call(ArgsList args, API *api) {
//        populating with arguments
    int size = arguments.size(), i;
    if (unlimited)
        size--;
    for (i = 0; i < size; ++i)
        api->setVariable(arguments[i], args[i]);
    if (unlimited) {
        auto last = std::vector<ObjPtr>(args.begin() + size, args.end());
        api->setVariable(arguments[size], New(Array(last)));
    }
    body->evaluate(api->interpreter);
}

bool ClassMethod::checkArguments(int count) {
    return function->checkArguments(count);
}

ObjPtr ClassMethod::call(ArgsList args, API *api) {
    api->setVariable("this", instance);
    return function->call(args, api);
}

ClassMethod::ClassMethod(Callable::ptr function, Instance::ptr instance) :
        function(function), instance(instance) {}
