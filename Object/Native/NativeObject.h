#ifndef INTERPRETER_NATIVEOBJECT_H
#define INTERPRETER_NATIVEOBJECT_H

#include "Native.h"

class BaseNative : public Object {
public:
//    item access
    virtual Object *getItem(Object *other);
    virtual Object *setItem(Object *other, Object *value);
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
    class NativeClass : public Class {
        Object *__call__(const std::vector<Object *> &args, Interpreter *interpreter) override {
            return new T();
        }
    };

    static std::map<std::string, NativeMethod *> methods;
protected:
    static void populate() {};

    static void addMethod(std::string name, nativeMethod method, int argumentsCount) {
        methods[name] = new NativeMethod(method, argumentsCount);
    }

public:
    Object *findAttribute(const std::string &name) override {
        auto result = Object::findAttribute(name);
        if (!result) {
            auto value = methods.find(name);
            if (value != methods.end()) {
                result = value->second;
            } else
                return nullptr;
        }
        auto method = dynamic_cast<Callable *> (result);
        if (method)
            return new ClassMethod(this, method);
        return result;
    }

    static Object *build() {
        T::populate();
        return new NativeClass();
    }
};

#endif //INTERPRETER_NATIVEOBJECT_H
