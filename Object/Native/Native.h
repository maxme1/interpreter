#ifndef INTERPRETER_NATIVE_H
#define INTERPRETER_NATIVE_H

#include <iostream>
#include <utility>
#include <cassert>
#include "../Callable.h"

// macros
#define New(object) ObjPtr(new Object)

#define $subclass(ClassName, Base) struct ClassName : public NativeObject<ClassName, Base> {\
    explicit ClassName(Class::ptr classPtr) : NativeObject(classPtr) {};

#define $class(ClassName) $subclass(ClassName, NoSuperClass)

#define $method(name, type) \
static ObjPtr (name)(ObjPtr _self, ArgsList args, API *api) { \
    auto self = type::cast(_self, true);

#define $lambda [](ArgsList args, API *api) -> ObjPtr

// types
typedef ObjPtr(*nativeFunction)(ArgsList, API *);
typedef ObjPtr(*nativeMethod)(ObjPtr, ArgsList, API *);

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
    ObjPtr call(ArgsList args, API *api) override;
};

typedef NativeCallable<nativeFunction> NativeFunction;
typedef NativeCallable<nativeMethod> NativeMethod;

#include "NativeObject.h"

#endif //INTERPRETER_NATIVE_H
