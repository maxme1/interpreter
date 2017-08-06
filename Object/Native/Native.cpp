#include "Native.h"


bool NativeCallable::checkArguments(int count) {
    return count >= minArguments and (count <= maxArguments or maxArguments == ANY);
}

NativeCallable::NativeCallable(int minArguments, int maxArguments) :
        minArguments(minArguments), maxArguments(maxArguments) {
    if (maxArguments == SAME)
        this->maxArguments = minArguments;
}

NativeFunction::NativeFunction(nativeFunction function, int minArguments, int maxArguments) :
        NativeCallable(minArguments, maxArguments), function(function) {}

Object *NativeFunction::__call__(ArgsList args, Interpreter *interpreter) {
    return function(args);
}

NativeMethod::NativeMethod(nativeMethod method, int minArguments, int maxArguments) :
        NativeCallable(minArguments, maxArguments), method(method) {}

Object *NativeMethod::__call__(const std::vector<Object *> &args, Interpreter *interpreter) {
    auto self = interpreter->getVariable("this");
    return method(self, args);
}
