#ifndef INTERPRETER_NATIVE_OBJECT_H
#define INTERPRETER_NATIVE_OBJECT_H

#include "Native.h"
#include "../Class.h"

struct NativeClass : public Class {
    explicit NativeClass(Class::ptr superclass) : Class(superclass) {}
};

struct NoSuperClass {
    static Class::ptr build() {
        return nullptr;
    }
};

template<typename T, typename Base>
class NativeObject : public Instance {
    friend class Class;
    struct LocalNative : public NativeClass {
        inline static Class::ptr getClass() {
            static Class::ptr instance = Class::ptr(new LocalNative(Base::build()));
            return instance;
        }

        LocalNative(LocalNative const &) = delete;
        void operator=(LocalNative const &)  = delete;

        Instance::ptr makeInstance(const ptr &instanceClass) override {
            if (instanceClass)
                return Instance::ptr(new T(instanceClass));
            auto s = getClass();
            return Instance::ptr(new T(getClass()));
        }

    private:
        explicit LocalNative(Class::ptr superclass) : NativeClass(superclass) {};
    };

    static bool populated;
protected:
    static void addMethod(const std::string &name, nativeMethod method, int argumentsCount = 0,
                          bool unlimited = false) {
        LocalNative::getClass()->setAttribute(name, New(NativeMethod(method, argumentsCount, unlimited)));
    }

public:
    explicit NativeObject(Class::ptr classPtr) : Instance(classPtr) {}

    NativeObject() : Instance(build()) {}

    static std::shared_ptr<T> cast(ObjPtr object, bool strict = false) {
        auto result = std::dynamic_pointer_cast<T>(object);
        assert(not strict or (result != nullptr));
        return result;
    }

    static void populate() {}

    static Class::ptr build() {
        auto result = LocalNative::getClass();
        if (!populated) {
            populated = true;
            T::populate();
        }
        return result;
    }
};

template<typename T, typename Base>
bool NativeObject<T, Base>::populated = false;


#endif //INTERPRETER_NATIVE_OBJECT_H
