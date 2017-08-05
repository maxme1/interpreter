#ifndef INTERPRETER_NATIVEOBJECT_H
#define INTERPRETER_NATIVEOBJECT_H

#include "Native.h"

class BaseNative : public Object {
public:
//    item access
    virtual Object *getItem(Object *other);
//    arithmetic
    virtual Object *add(Object *other);
    virtual Object *unary_add();
    virtual Object *subtract(Object *other);
    virtual Object *unary_subtract();
    virtual Object *multiply(Object *other);
    virtual Object *divide(Object *other);
//    comparison
    virtual Object *equal(Object *other);
    virtual Object *greater(Object *other);
    virtual Object *less(Object *other);
    virtual Object *greater_or_equal(Object *other);
    virtual Object *less_or_equal(Object *other);
    virtual Object *not_equal(Object *other);
};

template<typename T>
class NativeObject : public BaseNative {
    friend class Interpreter;

protected:
    static std::map<std::string, NativeMethod *> methods;

    static void addMethod(std::string name, nativeMethod method, int argumentsCount) {
        methods[name] = new NativeMethod(method, argumentsCount);
    }

public:
    Object *findAttribute(const std::string &name) {
        auto result = Object::findAttribute(name);
        if (!result) {
            auto value = methods.find(name);
            if (value != methods.end()) {
                result = value->second;
            }
        }
        auto method = dynamic_cast<Callable *> (result);
        if (method)
            return new ClassMethod(this, method);
        return result;
    }

    static Object *getInstance(ArgsList args) {
        return new T();
    }

    static Object *build() {
        return new NativeFunction(getInstance, 0);
    }
};

#endif //INTERPRETER_NATIVEOBJECT_H
