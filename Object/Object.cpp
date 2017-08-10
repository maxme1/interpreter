#include <iostream>
#include <utility>
#include "Object.h"
#include "Exception.h"

void Object::setAttribute(const std::string &name, ObjPtr value) {
    attributes[name] = value;
}

ObjPtr Object::findAttribute(const std::string &name) {
    auto value = attributes.find(name);
    if (value != attributes.end())
        return value->second;
    return nullptr;
}

ObjPtr Object::getAttribute(const std::string &name) {
    auto result = findAttribute(name);
    if (result)
        return result;
    throw Wrap(new AttributeError(name));
}