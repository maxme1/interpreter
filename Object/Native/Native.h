#ifndef INTERPRETER_NATIVE_H
#define INTERPRETER_NATIVE_H

#include <iostream>
#include <utility>
#include <cassert>
#include "../Callable.h"

// macros
#define $class(ClassName) struct ClassName : public NativeObject<ClassName, NoSuperClass> {\
    explicit ClassName(Class * classPtr) : NativeObject(classPtr) {};

#define $subclass(ClassName, Base) struct ClassName : public NativeObject<ClassName, Base> {\
    explicit ClassName(Class * classPtr) : NativeObject(classPtr) {};

#define $method(name, type) \
static Object *(name)(Object *_self, ArgsList args, API *api) { \
    auto self = type::cast(_self, true);

#define $lambda [](ArgsList args, API *api) -> Object *

// types
typedef Object *(*nativeFunction)(ArgsList, API *);
typedef Object *(*nativeMethod)(Object *, ArgsList, API *);

// callables
template<typename T>
class NativeCallable : public Callable {
    T function;
    int argumentsCount;
    bool unlimited;
protected:
    bool checkArguments(int count) override;

public:
    explicit NativeCallable(T function, int argumentsCount, bool unlimited = false);
    Object *call(ArgsList args, API *api) override;
};

typedef NativeCallable<nativeFunction> NativeFunction;
typedef NativeCallable<nativeMethod> NativeMethod;

#include "NativeObject.h"

#endif //INTERPRETER_NATIVE_H
