#include "Class.h"

#include <utility>
#include "Native/NativeObject.h"

// Instance

std::string Instance::asString() {
    return "<" + getClass()->asString() + " instance>";
}

ObjPtr Instance::findAttribute(const std::string &name) {
    auto result = Object::findAttribute(name);
    if (!result)
        result = getClass()->findAttribute(name);
    if (!result)
        return nullptr;
//        creating a class method
    auto method = std::dynamic_pointer_cast<Callable>(result);
    if (method)
        return New(ClassMethod(method, std::static_pointer_cast<Instance>(shared_from_this())));
    return result;
}

Instance::Instance(Class::ptr classPtr) : classPtr(classPtr) {}

Class::ptr Instance::getClass() {
    assert(classPtr);
    return classPtr;
}

// Class

Instance::ptr Class::makeInstance(const ptr &instanceClass) {
    if (instanceClass)
        return std::make_shared<Instance>(instanceClass);
    Class::ptr theClass(this);
    while (theClass->superclass)
        theClass = theClass->superclass;
    return theClass->makeInstance(std::static_pointer_cast<Class>(shared_from_this()));
}

Class::Class(const std::string &name, Scope::ptr body, Class::ptr superclass, Scope::ptr context) :
        superclass(superclass) {
    for (auto &&attribute : body->attributes)
        setAttribute(attribute.first, attribute.second);
}

ObjPtr Class::findAttribute(const std::string &name) {
    auto result = Object::findAttribute(name);
    if (!result and superclass)
        return superclass->findAttribute(name);
    return result;
}

Class::ptr Class::getSuperClass() {
    return superclass;
}

std::string Class::asString() {
    auto name = findAttribute("@name");
    std::string result = "<class";
    if (name)
        result += " " + name->asString();
    return result + ">";
}

Class::Class(Class::ptr superclass) : superclass(superclass) {}