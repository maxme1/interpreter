#include "Native.h"

template<typename T>
bool NativeCallable<T>::checkArguments(int count) {
    return count == argumentsCount or (count >= argumentsCount and unlimited);
}

template<typename T>
NativeCallable<T>::NativeCallable(T function, int argumentsCount, bool unlimited) :
        argumentsCount(argumentsCount), unlimited(unlimited), function(function) {}

template<typename T>
Object *NativeCallable<T>::__call__(ArgsList args, API *api) {
    return function(args, api);
}

template<>
Object *NativeCallable<nativeMethod>::__call__(const std::vector<Object *> &args, API *api) {
    auto self = api->getVariable("this");
    return function(self, args, api);
}

template
class NativeCallable<nativeFunction>;
template
class NativeCallable<nativeMethod>;