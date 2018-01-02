#ifndef INTERPRETER_NATIVE_H
#define INTERPRETER_NATIVE_H

#include <iostream>
#include <utility>
#include <cassert>
#include "../Callable.h"
#include "../Class.h"
#include "../../TreeWalk/Interpreter/Interpreter.h"

// macros
#define New(object) ObjPtr(new object)
#define $this(ClassName) std::static_pointer_cast<ClassName>(shared_from_this())

#define $subclass(ClassName, Base) struct ClassName : public NativeObject<ClassName, Base> {\
    explicit ClassName(Class::ptr classPtr) : NativeObject(classPtr) {};

#define $class(ClassName) $subclass(ClassName, NoSuperClass)

#define $method(name, type) \
static ObjPtr (name)(Interpreter * interpreter, ObjPtr _self, ArgsList positional, KwargsList keyword) { \
    auto self = type::cast(_self, true);

#define $lambda [](Interpreter *interpreter, ArgsList positional, KwargsList keyword) -> ObjPtr

// types
typedef ObjPtr(*nativeFunction)(Interpreter *, ArgsList, KwargsList);
typedef ObjPtr(*nativeMethod)(Interpreter *, ObjPtr, ArgsList, KwargsList);

// callables
template<typename T>
class NativeCallable : public FunctionPrototype {
    T function;

public:
    explicit NativeCallable(T function, std::vector<std::string> &arguments, std::map<std::string, ObjPtr> defaults);
    ObjPtr call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) override;
};

typedef NativeCallable<nativeFunction> NativeFunction;
typedef NativeCallable<nativeMethod> NativeMethod;

#include "NativeObject.h"

#endif //INTERPRETER_NATIVE_H
