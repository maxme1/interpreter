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
struct name : public NativeObject<name>

// types
typedef Object *(*nativeFunction)(ArgsList);
typedef Object *(*nativeMethod)(Object *, ArgsList);

// callables
class NativeCallable : public Callable {
    int minArguments, maxArguments;
protected:
    bool checkArguments(int count) override;

public:
    static const int ANY = -1, SAME = -2;
    explicit NativeCallable(int minArguments, int maxArguments);
};

class NativeFunction : public NativeCallable {
    nativeFunction function;
public:
    NativeFunction(nativeFunction function, int minArguments, int maxArguments = NativeCallable::SAME);
    Object *__call__(ArgsList args, Interpreter *interpreter) override;
};

class NativeMethod : public NativeCallable {
    nativeMethod method;
public:
    NativeMethod(nativeMethod method, int minArguments, int maxArguments = NativeCallable::SAME);
    Object *__call__(ArgsList args, Interpreter *interpreter) override;
};

#include "NativeObject.h"

#endif //INTERPRETER_NATIVE_H
