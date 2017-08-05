#include "Native.h"


bool NativeFunction::checkArguments(int count) { return argumentsCount == count; }

Object *NativeFunction::__call__(const std::vector<Object *> &args, Interpreter *interpreter) {
    return function(args);
}

bool NativeMethod::checkArguments(int count) {
    return argumentsCount == count;
}

Object *NativeMethod::__call__(const std::vector<Object *> &args, Interpreter *interpreter) {
    auto self = interpreter->getVariable("this");
    return method(self, args);
}
