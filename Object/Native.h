#ifndef INTERPRETER_NATIVE_H
#define INTERPRETER_NATIVE_H

#include <iostream>
#include <utility>
#include "Types/Callable.h"
#include "Class.h"
#include <cassert>

typedef Object *(*nativeFunction)(Object *);
class NativeFunction : public Callable {
    std::vector<std::string> arguments;
    nativeFunction function;

protected:
    bool checkArguments(int count) override {
        return arguments.size() == count;
    }

    std::string argument(int i) override { return arguments[i]; }

    Object *__call__(Object *args, Interpreter *interpreter) override {
        return function(args);
    }

public:
    NativeFunction(nativeFunction function, std::vector<std::string> arguments) :
            arguments(std::move(arguments)), function(function) {}
};

struct NativeClass;
typedef std::vector<Object *> &ArgsList;
typedef Object *(*nativeMethod)(NativeClass *, ArgsList);
#define cast(a) auto self = dynamic_cast<a *>(_self); assert(self);
struct NativeClass : public Object {
    std::map<std::string, std::pair<nativeMethod, std::vector<std::string>>> methods;

    Object *findAttribute(const std::string &name) override;
};

class NativeMethod : public Callable {
    friend class Interpreter;
    nativeMethod method;
    NativeClass *source;
    std::vector<std::string> arguments;
protected:
    bool checkArguments(int count) override {
        return arguments.size() == count;
    }

    std::string argument(int i) override { return arguments[i]; }

    Object *__call__(Object *args, Interpreter *interpreter) override {
        auto localArgs = std::vector<Object *>(arguments.size());
        for (auto &&item : arguments) {
            localArgs.push_back(args->getAttribute(item));
        }
        return method(source, localArgs);
    }

public:
    NativeMethod(NativeClass *source, nativeMethod method, std::vector<std::string> arguments) :
            method(method), source(source), arguments(std::move(arguments)) {
        source->save();
    }

    ~NativeMethod() override {
        if (source->canDelete())
            delete source;
    }
};

#endif //INTERPRETER_NATIVE_H
