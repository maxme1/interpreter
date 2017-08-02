#include <iostream>
#include "Object.h"
#include "Types/Bool.h"
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

Object *Object::getAttribute(const std::string &name) {
    auto value = attributes.find(name);
    if (value != attributes.end())
        return value->second;
    throw Exception("AttributeError");
}

Object *Object::not_equal(Object *other) { throw Exception("Operator not defined"); }

Object *Object::less_or_equal(Object *other) { throw Exception("Operator not defined"); }

Object *Object::greater_or_equal(Object *other) { throw Exception("Operator not defined"); }

Object *Object::less(Object *other) { throw Exception("Operator not defined"); }

Object *Object::greater(Object *other) { throw Exception("Operator not defined"); }

Object *Object::equal(Object *other) { throw Exception("Operator not defined"); }

Object *Object::divide(Object *other) { throw Exception("Operator not defined"); }

Object *Object::multiply(Object *other) { throw Exception("Operator not defined"); }

Object *Object::unary_subtract() { throw Exception("Operator not defined"); }

Object *Object::subtract(Object *other) { throw Exception("Operator not defined"); }

Object *Object::unary_add() { throw Exception("Operator not defined"); }

Object *Object::add(Object *other) { throw Exception("Operator not defined"); }

