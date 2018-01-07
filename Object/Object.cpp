#include <iostream>
#include <utility>
#include <cassert>
#include "Object.h"
#include "Exception.h"

void Object::setAttribute(const std::string &name, ObjPtr value) {
    attributes[name] = value;
}

ObjPtr Object::findAttribute(const std::string &name) {
    auto value = attributes.find(name);
    if (value != attributes.end()) {
        assert(!value->second.expired());
        return value->second.lock();
    }
    return nullptr;
}

ObjPtr Object::getAttribute(const std::string &name) {
    auto result = findAttribute(name);
    if (!result)
        throw Interpreter::ExceptionWrapper(new AttributeError(name));
    return result;
}