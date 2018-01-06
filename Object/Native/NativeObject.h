#ifndef INTERPRETER_NATIVE_OBJECT_H
#define INTERPRETER_NATIVE_OBJECT_H

#include "Native.h"
#include "../Class.h"
//
//struct NativeClass : public Class {
//    explicit NativeClass(Class::ptr superclass) : Class(superclass) {}
//};

struct NoSuperClass {
    static Class::ptr build() {
        return nullptr;
    }
};

template<typename T, typename Base>
class NativeObject : public Instance {
    friend class Class;
    struct LocalNative : public Class {
        inline static Class::ptr getClass() {
            static Class::ptr instance = Class::ptr(new LocalNative(Base::build()));
            return instance;
        }

        LocalNative(LocalNative const &) = delete;
        void operator=(LocalNative const &)  = delete;

        ObjPtr makeInstance(Class::ptr base = nullptr) override {
            if (base)
                return Instance::ptr(new T(base));
            return Instance::ptr(new T(getClass()));
        }

    private:
        explicit LocalNative(Class::ptr superclass) : Class(superclass) {};
    };

    static bool populated;
protected:
    static void addMethod(const std::string &name, nativeMethod method, int argumentsCount = 0, bool variable = false) {
        auto arguments = std::vector<FunctionPrototype::Argument>();
        for (int i = 0; i < argumentsCount; ++i)
            arguments.emplace_back("arg" + std::to_string(i), nullptr, true, variable and i == argumentsCount - 1);
        LocalNative::getClass()->setAttribute(name, New(NativeMethod(method, arguments)));
    }

//    static void addMethod(const std::string &name, nativeMethod method, std::vector<std::string> &arguments,
//                          std::map<std::string, ObjPtr> &defaults) {
//        LocalNative::getClass()->setAttribute(name, New(NativeMethod(method, arguments, defaults)));
//    }

public:
    explicit NativeObject(Class::ptr classPtr) : Instance(classPtr) {}

    NativeObject() : Instance(build()) {}

    static std::shared_ptr<T> cast(ObjPtr object, bool strict = true) {
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
