#include "Exception.h"
#include "Types/String.h"

ObjPtr Exception::init(Interpreter *interpreter, ObjPtr _self, ArgsList positional, KwargsList keyword) {
//    TODO: add defaults
    if (positional.empty())
        _self->setAttribute("message", New(String("")));
    else
        _self->setAttribute("message", positional[0]);
    return nullptr;
}

ObjPtr Exception::str(Interpreter *interpreter, ObjPtr _self, ArgsList positional, KwargsList keyword) {
    return New(String(Exception::getString(_self)));
}

void Exception::setMessage(Object *target, const std::string &message) {
    target->setAttribute("message", New(String(message)));
}

std::string Exception::getString(ObjPtr target) {
    auto message = target->getAttribute("message")->asString();
    auto name = target->getAttribute("@name")->asString();
    return name + ": " + message;
}

std::string Exception::asString() {
    return getString(shared_from_this());
}

void Exception::setName(ObjPtr target, const std::string &name) {
    target->setAttribute("@name", New(String(name)));
}
