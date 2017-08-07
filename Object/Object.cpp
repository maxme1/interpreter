#include <iostream>
#include "Object.h"
#include "Types/Exception.h"

Object::~Object() {
    for (auto attribute : attributes) {
        if (attribute.second->canDelete())
            delete attribute.second;
    }
}

bool Object::canDelete() {
    return mentions <= 0;
}

void Object::setAttribute(const std::string &name, Object *value) {
    value->save();
//    remove the old object
    auto pair = attributes.find(name);
    if (pair != attributes.end())
        if (pair->second->canDelete())
            delete pair->second;
    attributes[name] = value;
}

Object *Object::findAttribute(const std::string &name) {
    auto value = attributes.find(name);
    if (value != attributes.end())
        return value->second;
    return nullptr;
}

Object *Object::getAttribute(const std::string &name) {
    auto result = findAttribute(name);
    if (result)
        return result;
    throw Exception("AttributeError");
}
