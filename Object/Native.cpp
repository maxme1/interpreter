#include "Native.h"

Object *NativeClass::findAttribute(const std::string &name) {
    auto result = Object::findAttribute(name);
    if (!result) {
        auto value = methods.find(name);
        if (value != methods.end()) {
            auto pair = value->second;
            return new NativeMethod(this, pair.first, pair.second);
        }
    }
    return result;
}