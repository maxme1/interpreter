#include "Exception.h"
#include "String.h"

Object *Exception::init(Object *_self, ArgsList args, API *api) {
    _self->setAttribute("message", args[0]);
    return nullptr;
}

Object *Exception::str(Object *_self, ArgsList args, API *api) {
    return new String(Exception::getString(_self));
}

void Exception::setMessage(Object *target, const std::string &message) {
    target->setAttribute("message", new String(message));
}

std::string Exception::getString(Object *target) {
    auto message = target->getAttribute("message")->asString();
    auto name = target->getAttribute("@name")->asString();
    return name + ": " + message;
}

std::string Exception::asString() {
    return getString(this);
}

void Exception::setName(Object *target, const std::string &name) {
    target->setAttribute("@name", new String(name));
}
