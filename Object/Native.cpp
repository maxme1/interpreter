#include "Native.h"

Object *NativeClass::findAttribute(const std::string &name) {
    auto result = Object::findAttribute(name);
    if (!result) {
        auto value = methods.find(name);
        if (value != methods.end()) {
            return value->second;
        }
    }
    return result;
}

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
