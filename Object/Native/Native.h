#ifndef INTERPRETER_NATIVE_H
#define INTERPRETER_NATIVE_H

#include <iostream>
#include <utility>
#include "../Callable.h"
#include "../Class.h"
#include <cassert>

// some useful macros
#define $method(name, type)\
static Object *(name)(Object *_self, ArgsList args) {\
    auto self = dynamic_cast<type *>(_self); \
    assert(self);

#define $class(name) \
template<typename name> \
std::map<std::string, NativeMethod *> NativeObject<name>::methods; \
struct Array : public NativeObject<name>

// types
typedef Object *(*nativeFunction)(ArgsList);
typedef Object *(*nativeMethod)(Object *, ArgsList);

// callables
class NativeFunction : public Callable {
    int argumentsCount;
    nativeFunction function;

protected:
    bool checkArguments(int count) override;
    Object *__call__(ArgsList args, Interpreter *interpreter) override;

public:
    NativeFunction(nativeFunction function, int argumentsCount) : argumentsCount(argumentsCount), function(function) {}
};

class NativeMethod : public Callable {
    nativeMethod method;
    int argumentsCount;
protected:
    bool checkArguments(int count) override;
    Object *__call__(ArgsList args, Interpreter *interpreter) override;

public:
    NativeMethod(nativeMethod method, int argumentsCount) : method(method), argumentsCount(argumentsCount) {}
};

#include "NativeObject.h"

#endif //INTERPRETER_NATIVE_H
