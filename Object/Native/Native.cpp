#include "Native.h"

template<typename T>
bool NativeCallable<T>::checkArguments(int count) {
    return count == argumentsCount or (count >= argumentsCount and unlimited);
}

template<typename T>
NativeCallable<T>::NativeCallable(T function, int argumentsCount, bool unlimited) :
        argumentsCount(argumentsCount), unlimited(unlimited), function(function) {}

template<typename T>
ObjPtr NativeCallable<T>::call(ArgsList args, Interpreter *interpreter) {
    return function(args, interpreter);
}

//template<>
//ObjPtr NativeCallable<nativeMethod>::call(const std::vector<ObjPtr> &args, Interpreter *interpreter) {
//    auto self = interpreter->getVariable("this");
//    return function(self, args, interpreter);
//}

template class NativeCallable<nativeFunction>;
//template
//class NativeCallable<nativeMethod>;