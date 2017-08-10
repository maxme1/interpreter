#include "Class.h"
#include "Native/NativeObject.h"

// Instance

std::string Instance::asString() {
    return "<" + getClass()->asString() + " instance>";
}

Object *Instance::findAttribute(const std::string &name) {
    auto result = Object::findAttribute(name);
    if (!result)
        result = getClass()->findAttribute(name);
    if (!result)
        return nullptr;
//        creating a class method
    auto method = dynamic_cast<Callable *> (result);
    if (method)
        return new ClassMethod(method, this);
    return result;
}

Instance::Instance(Class *classPtr) : classPtr(classPtr) {
    assert(classPtr);
    classPtr->save();
}

Instance::~Instance() {
    Object::remove(classPtr);
}

Class *Instance::getClass() {
    assert(classPtr);
    return classPtr;
}

// Class

Object *Class::makeInstance(Class *instanceClass) {
    if (instanceClass)
        return new Instance(instanceClass);
    Class *theClass = this;
    while (theClass->superclass != nullptr)
        theClass = theClass->superclass;
    return theClass->makeInstance(this);
}

Class::Class(const std::string &name, Scope *body, Class *superclass, Scope *context) {
    for (auto &&attribute : body->attributes)
        setAttribute(attribute.first, attribute.second);

//    TODO: refactor
    if (!superclass) {
        this->superclass = nullptr;
        return;
    }
    this->superclass = superclass;
    this->superclass->save();
}

Class::~Class() {
    Object::remove(superclass);
}

Object *Class::findAttribute(const std::string &name) {
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

