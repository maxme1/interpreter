#ifndef INTERPRETER_EXCEPTION_H
#define INTERPRETER_EXCEPTION_H

#include "Native/Native.h"
#include "Types/String.h"

#define $exception(ClassName, Base) \
$subclass(ClassName, Base) \
    explicit ClassName(const std::string &body) { \
        Exception::setMessage(this, body); \
    } \
};

$class(Exception)

    explicit Exception(const std::string &body) {
        Exception::setMessage(this, body);
    }

    static ObjPtr init(Interpreter *interpreter, ObjPtr _self, ArgsList positional, KwargsList keyword);
    static ObjPtr str(Interpreter *interpreter, ObjPtr _self, ArgsList positional, KwargsList keyword);
    static void setMessage(Object *target, const std::string &message);

    static void populate() {
        addMethod("init", init, 1, true);
        addMethod("str", str);
    }
};

$exception(AccessError, Exception)
$exception(AttributeError, AccessError)
$exception(VariableError, AccessError)
$exception(IndexError, AccessError)

$exception(ArithmeticError, Exception)
$exception(ImportError, Exception)
$exception(ValueError, Exception)

#endif //INTERPRETER_EXCEPTION_H
