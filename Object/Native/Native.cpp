#include "Native.h"

template<typename T>
NativeCallable<T>::NativeCallable(T function, int argumentsCount, bool unlimited) :
        argumentsCount(argumentsCount), unlimited(unlimited), function(function) {}

template<typename T>
ObjPtr NativeCallable<T>::call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) {
//    TODO: implement
    return function(interpreter, positional);
}

template<typename T>
bool NativeCallable<T>::checkArguments(ArgsList positional, KwargsList keyword) {
    return keyword.empty() and positional.size() == argumentsCount;
}

template<>
ObjPtr NativeCallable<nativeMethod>::call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) {
    auto self = interpreter->getVariable("this", 0);
//    TODO: implement
    return function(interpreter, self, positional);
}

template
class NativeCallable<nativeFunction>;
template
class NativeCallable<nativeMethod>;
