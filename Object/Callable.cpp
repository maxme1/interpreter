#include "Callable.h"
#include "Types/Array.h"
#include "../Parser/Statement/Statement.h"

Callable::Callable(Scope *context) : context(context) {
    context->save();
}

Callable::~Callable() {
    Object::remove(context);
}

Function::Function(std::vector<std::string> arguments, Statement *body, bool unlimited, Scope *context) :
        Callable(context), body(body), arguments(std::move(arguments)), unlimited(unlimited) {}

bool Function::checkArguments(int count) {
    int size = arguments.size();
    if (unlimited)
        size--;
    bool t = size == count or (arguments.size() <= count and unlimited);
    if (!t)
        std::cout << "expected: " << arguments.size();
    return t;
}

Object *Function::call(ArgsList args, API *api) {
//        populating with arguments
    int size = arguments.size(), i;
    if (unlimited)
        size--;
    for (i = 0; i < size; ++i)
        api->setVariable(arguments[i], args[i]);
    if (unlimited) {
        auto last = std::vector<Object *>(args.begin() + size, args.end());
        api->setVariable(arguments[size], new Array(last));
    }
    body->evaluate(api->interpreter);
}

Function::~Function() {
    delete body;
}

bool ClassMethod::checkArguments(int count) {
    return function->checkArguments(count);
}

Object *ClassMethod::call(ArgsList args, API *api) {
    api->setVariable("this", instance);
    return function->call(args, api);
}

ClassMethod::ClassMethod(Callable *function, Instance *instance) :
        function(function), instance(instance) {
    function->save();
    instance->save();
}

ClassMethod::~ClassMethod() {
    Object::remove(function);
    Object::remove(instance);
}