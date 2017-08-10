#ifndef INTERPRETER_NATIVEOBJECT_H
#define INTERPRETER_NATIVEOBJECT_H

#include "Native.h"
#include "../Class.h"

struct NativeClass : public Class {
    explicit NativeClass(Class *superclass) : Class(superclass) {}
};

struct NoSuperClass {
    static Class *build() {
        return nullptr;
    }
};

template<typename T, typename Base>
class NativeObject : public Instance {
    friend class Class;
    struct LocalNative : public NativeClass {
        inline static LocalNative &getClass() {
            static LocalNative LocalClass(Base::build());
            return LocalClass;
        }

        LocalNative(LocalNative const &) = delete;
        void operator=(LocalNative const &)  = delete;

        Object *makeInstance(Class *instanceClass) override {
            if (instanceClass)
                return new T(instanceClass);
            return new T(&getClass());
        }

    private:
        explicit LocalNative(Class *superclass) : NativeClass(superclass) {};
    };

    static bool populated;
protected:
    static void addMethod(const std::string &name, nativeMethod method, int argumentsCount = 0,
                          bool unlimited = false) {
        LocalNative::getClass().setAttribute(name, new NativeMethod(method, argumentsCount, unlimited));
    }

public:
    explicit NativeObject(Class *classPtr) : Instance(classPtr) {}

    NativeObject() : Instance(build()) {}

    static T *cast(Object *object, bool strict = false) {
        auto result = dynamic_cast<T *>(object);
        assert(not strict or (result != nullptr));
        return result;
    }

    static void populate() {}

    static Class *build() {
        auto result = &LocalNative::getClass();
        if (!populated) {
            populated = true;
            T::populate();
        }
        return result;
    }
};

template<typename T, typename Base>
bool NativeObject<T, Base>::populated = false;


#endif //INTERPRETER_NATIVEOBJECT_H
