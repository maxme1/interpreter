#include "Exception.h"
#include "Types/String.h"

ObjPtr Exception::init(Interpreter *interpreter, ObjPtr _self, ArgsList positional, KwargsList keyword) {
    std::string message;
    if (!positional.empty())
        message = String::str(positional[0], interpreter)->string;
    setMessage(_self.get(), message);
    return nullptr;
}

ObjPtr Exception::str(Interpreter *interpreter, ObjPtr _self, ArgsList positional, KwargsList keyword) {
    return _self->getAttribute("body");
}

void Exception::setMessage(Object *target, const std::string &message) {
    assert(target);
    target->setAttribute("message", New(String(message)));
    auto result = dynamic_cast<Instance *>(target)->getClass()->getName();
    if (!message.empty())
        result += ": " + message;
    target->setAttribute("body", New(String(result)));
}