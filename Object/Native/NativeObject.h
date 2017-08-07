#ifndef INTERPRETER_NATIVEOBJECT_H
#define INTERPRETER_NATIVEOBJECT_H

#include "Native.h"

template<typename T>
class NativeObject : public Object {
    struct NativeClass : public Class {
        inline static NativeClass &getClass() {
            static NativeClass Instance;
            return Instance;
        }

        NativeClass(NativeClass const &) = delete;
        void operator=(NativeClass const &)  = delete;
        static bool populated;
    protected:
        Object *__call__(const std::vector<Object *> &args, API *api) override {
            return new T();
        }

    private:
        NativeClass() = default;
    };

    static Object *classPtr;
protected:
    static void addMethod(const std::string &name, nativeMethod method, int argumentsCount = 0,
                          bool unlimited = false) {
        classPtr->setAttribute(name, new NativeMethod(method, argumentsCount, unlimited));
    }

public:
    static T *cast(Object *object, bool strict = false) {
        auto result = dynamic_cast<T *>(object);
        if (strict and !result)
            throw Exception("Could not convert object");
        return result;
    }

    static void populate() {}

    Object *findAttribute(const std::string &name) override {
        auto result = Object::findAttribute(name);
        if (!result and classPtr)
            result = classPtr->findAttribute(name);
        if (!result)
            return nullptr;
//        creating a class method
        auto method = dynamic_cast<Callable *> (result);
        if (method)
            return new ClassMethod(this, method);
        return result;
    }

    static Object *getClass() {
        if (!classPtr) {
            classPtr = &NativeClass::getClass();
            T::populate();
        }
        return classPtr;
    }
};

template<typename T>
Object *NativeObject<T>::classPtr = nullptr;

#endif //INTERPRETER_NATIVEOBJECT_H
