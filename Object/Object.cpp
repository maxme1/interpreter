#include <iostream>
#include "Object.h"
#include "Bool.h"


Object::~Object() {
    for (auto attribute : attributes) {
        if (attribute.second->canDelete())
            delete attribute.second;
    }
}

bool Object::zombie() {
    return mentions <= 0;
}

bool Object::canDelete() {
    mentions--;
    return zombie();
}

Bool *Object::__bool__() {
    return new Bool(asBool());
}

void Object::setAttribute(std::string name, Object *value) {
    auto pair = attributes.find(name);
    if (pair != attributes.end()) {
//    remove the old object
        if (pair->second != value) {
            if (pair->second->canDelete())
                delete pair->second;
            value->mentions++;
        }
    } else
        value->mentions++;
    attributes[name] = value;
}

Object *Object::getAttribute(std::string name) {
    auto value = attributes.find(name);
    if (value != attributes.end())
        return value->second;
//    TODO: need custom exceptions
    throw "AttributeError";
}
