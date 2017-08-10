#ifndef INTERPRETER_EXCEPTION_H
#define INTERPRETER_EXCEPTION_H

#include "../Native/Native.h"

#define $exception(ClassName, Base) \
$subclass(ClassName, Base) \
    explicit ClassName(const std::string &body) { \
        Exception::setMessage(this, body); \
    } \
    static void populate(){ \
        Exception::setName(ClassName::build(), #ClassName); \
    } \
    std::string asString() override { \
        return Exception::getString(this); \
    } \
};

$class(Exception)
    explicit Exception(const std::string &body) {
        Exception::setMessage(this, body);
    }

    static Object *init(Object *_self, ArgsList args, API *api);
    static Object *str(Object *_self, ArgsList args, API *api);
    std::string asString() override;

    static void setMessage(Object *target, const std::string &message);
    static void setName(Object *target, const std::string &name);
    static std::string getString(Object *target);

    static void populate() {
        addMethod("init", init, 1);
        addMethod("str", str);
    }
};

$exception(AttributeError, Exception)
$exception(ValueError, Exception)
$exception(SyntaxError, Exception)

#endif //INTERPRETER_EXCEPTION_H
