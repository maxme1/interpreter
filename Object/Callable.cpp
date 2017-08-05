#include "Callable.h"

Callable::Callable(Scope *context) : context(context) {
    context->save();
}

Callable::~Callable() {
    if (context and context->canDelete())
        delete context;
}

bool Function::checkArguments(int count) { return arguments.size() == count; }


Object *Function::__call__(const std::vector<Object *> &args, Interpreter *interpreter) {
//        populating with arguments
    for (int i = 0; i < arguments.size(); ++i)
        interpreter->setVariable(arguments[i], args[i]);

    body->evaluate(interpreter);
}

Function::~Function() {
    delete body;
}

bool ClassMethod::checkArguments(int count) {
    return function->checkArguments(count);
}

Object *ClassMethod::__call__(const std::vector<Object *> &args, Interpreter *interpreter) {
    interpreter->setVariable("this", source);
    return function->__call__(args, interpreter);
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