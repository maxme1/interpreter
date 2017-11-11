#ifndef INTERPRETER_NATIVE_H
#define INTERPRETER_NATIVE_H

#include <iostream>
#include <utility>
#include <cassert>
#include "../Callable.h"
#include "../../TreeWalk/Interpreter/Interpreter.h"

// macros
#define New(object) ObjPtr(new object)
#define $this(ClassName) std::static_pointer_cast<ClassName>(shared_from_this())

#define $subclass(ClassName, Base) struct ClassName : public NativeObject<ClassName, Base> {\
    explicit ClassName(Class::ptr classPtr) : NativeObject(std::move(classPtr)) {};

#define $class(ClassName) $subclass(ClassName, NoSuperClass)

#define $method(name, type) \
static ObjPtr (name)(ObjPtr _self, ArgsList args, API *api) { \
    auto self = type::cast(_self, true);

#define $lambda [](ArgsList args, Interpreter *interpreter) -> ObjPtr

// types
typedef ObjPtr(*nativeFunction)(ArgsList, Interpreter *);
//typedef ObjPtr(*nativeMethod)(ObjPtr, ArgsList, Interpreter *);

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
    ObjPtr call(ArgsList args, Interpreter *interpreter) override;
};

typedef NativeCallable<nativeFunction> NativeFunction;
//typedef NativeCallable<nativeMethod> NativeMethod;

#include "NativeObject.h"

#endif //INTERPRETER_NATIVE_H
