#include "Object.h"


// TODO: need garbage collection
void Object::setAttribute(std::string name, Object *value) {
    attributes[name] = value;
}

Object *Object::getAttribute(std::string name) {
    auto value = attributes.find(name);
    if (value != attributes.end())
        return value->second;
    return nullptr;
}

Number::Number(int value) : value(value) {}

std::string Number::str() {
    return std::to_string(value);
}

Object *Number::add(Object *other) {
    Number *local = dynamic_cast<Number *>(other);
    if (local != NULL) {
        return new Number(value + local->value);
    }
//    TODO: also raise
    return nullptr;
}

Object *Number::uadd() {
    return this;
}

Object *Number::usub() {
    return new Number(-value);
}

Object *Number::sub(Object *other) {
    Number *local = dynamic_cast<Number *>(other);
    if (local != NULL) {
        return new Number(value - local->value);
    }
//    TODO: also raise
    return nullptr;
}

Object *Number::mul(Object *other) {
    Number *local = dynamic_cast<Number *>(other);
    if (local != NULL) {
        return new Number(value * local->value);
    }
    return nullptr;
}

Object *Number::div(Object *other) {
    Number *local = dynamic_cast<Number *>(other);
    if (local != NULL) {
        return new Number(value / local->value);
    }
    return nullptr;
}
