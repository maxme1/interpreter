#include "Callable.h"
#include "Types/Array.h"

Callable::Callable(Object *context) : context(context) {
    context->save();
}

Callable::~Callable() {
    if (context and context->canDelete())
        delete context;
}

Function::Function(std::vector<std::string> arguments, Statement *body, bool unlimited, Object *context) :
        Callable(context), body(body), arguments(std::move(arguments)), unlimited(unlimited) {}

bool Function::checkArguments(int count) {
    int size = arguments.size();
    if (unlimited)
        size--;
    return size == count or (arguments.size() <= count and unlimited);
}

Object *Function::__call__(ArgsList args, API *api) {
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

Object *ClassMethod::__call__(ArgsList args, API *api) {
    api->setVariable("this", source);
//    TODO: better inheritance ramification
    auto instance = dynamic_cast<Instance *>(source);
    if (instance) {
        auto super = instance->getSuperClass();
        if (super)
            api->setVariable("super", super);
    }
    return function->__call__(args, api);
}

ClassMethod::ClassMethod(Object *source, Callable *function) :
        function(function), source(source) {
    function->save();
    source->save();
}

ClassMethod::~ClassMethod() {
    if (function->canDelete())
        delete function;
    if (source->canDelete())
        delete source;
}