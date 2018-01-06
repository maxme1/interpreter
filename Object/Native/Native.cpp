#include "Native.h"

template<typename T>
NativeCallable<T>::NativeCallable(T function, std::vector<FunctionPrototype::Argument> &arguments) :
        FunctionPrototype(arguments), function(function) {}

template<typename T>
ObjPtr NativeCallable<T>::call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) {
    return function(interpreter, positional, keyword);
}

template<>
ObjPtr NativeCallable<nativeMethod>::call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) {
    auto self = interpreter->getVariable("this", 0);
    return function(interpreter, self, positional, keyword);
}

template class NativeCallable<nativeFunction>;
template class NativeCallable<nativeMethod>;
