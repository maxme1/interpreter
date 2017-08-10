#include "Class.h"
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
    auto method = std::dynamic_pointer_cast<Callable *>(result);
    if (method)
        return new ClassMethod(method, this);
    return result;
}

Instance::Instance(Class *classPtr) : classPtr(classPtr) {}

Class *Instance::getClass() {
    assert(classPtr);
    return classPtr;
}

// Class

ObjPtr Class::makeInstance(Class *instanceClass) {
    if (instanceClass)
        return new Instance(instanceClass);
    Class *theClass = this;
    while (theClass->superclass != nullptr)
        theClass = theClass->superclass;
    return theClass->makeInstance(this);
}

Class::Class(const std::string &name, Scope *body, Class *superclass, Scope *context) : superclass(superclass) {
    for (auto &&attribute : body->attributes)
        setAttribute(attribute.first, attribute.second);
}

ObjPtr Class::findAttribute(const std::string &name) {
    auto result = Object::findAttribute(name);
    if (!result and superclass)
        return superclass->findAttribute(name);
    return result;
}

Class *Class::getSuperClass() {
    return superclass;
}

std::string Class::asString() {
    auto name = findAttribute("@name");
    std::string result = "<class";
    if (name)
        result += " " + name->asString();
    return result + ">";
}

Class::Class(Class *superclass) : superclass(superclass) {}