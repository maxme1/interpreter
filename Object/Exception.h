#ifndef INTERPRETER_EXCEPTION_H
#define INTERPRETER_EXCEPTION_H

#include "Native/Native.h"

#define $exception(ClassName, Base) \
$subclass(ClassName, Base) \
    explicit ClassName(const std::string &body) { \
        Exception::setMessage(this, body); \
    } \
    static void populate(){ \
        Exception::setName(ClassName::build(), #ClassName); \
    } \
    std::string asString() override { \
        return Exception::getString(shared_from_this()); \
    } \
};

$class(Exception)

    explicit Exception(const std::string &body) {
        Exception::setMessage(this, body);
    }

    static ObjPtr init(Interpreter *interpreter, ObjPtr _self, ArgsList positional, KwargsList keyword);
    static ObjPtr str(Interpreter *interpreter, ObjPtr _self, ArgsList positional, KwargsList keyword);
    std::string asString() override;

    static void setMessage(Object *target, const std::string &message);
    static void setName(ObjPtr target, const std::string &name);
    static std::string getString(ObjPtr target);

    static void populate() {
        addMethod("init", init, 1);
        addMethod("str", str);
        Exception::setName(Exception::build(), "Exception");
    }
};

$exception(AccessError, Exception)
$exception(AttributeError, AccessError)
$exception(VariableError, AccessError)
$exception(IndexError, AccessError)

$exception(ImportError, Exception)
$exception(ValueError, Exception)
$exception(SyntaxError, Exception)

#endif //INTERPRETER_EXCEPTION_H
