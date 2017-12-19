#include "Class.h"

#include "Native/NativeObject.h"
#include "Native/Native.h"

// Instance

std::string Instance::asString() {
    return "<" + getClass()->asString() + " instance>";
}

ObjPtr Instance::findAttribute(const std::string &name) {
    auto result = Object::findAttribute(name);
    if (!result)
        result = getClass()->findAttribute(name);
//     creating a class method
    assert(result);
    auto method = std::dynamic_pointer_cast<Callable>(result);
    if (method)
        return ObjPtr(new ClassMethod(method, $this(Instance)));
    return result;
}

Instance::Instance(Class::ptr theClass) : theClass(std::move(theClass)) {}

Class::ptr Instance::getClass() {
    assert(theClass);
    return theClass;
}

// Class

Class::Class(const std::string &name, Scope::ptr body, Class::ptr superclass, Scope::ptr closure) :
        Callable(closure), superClass(superclass) {
//    copy the scope
    for (auto &&attribute : body->attributes)
        setAttribute(attribute.first, attribute.second);
}

ObjPtr Class::findAttribute(const std::string &name) {
    auto result = Object::findAttribute(name);
    if (!result and superClass)
        return superClass->findAttribute(name);
    return result;
}

std::string Class::asString() {
    return "<class>";
}

ObjPtr Class::makeInstance(Class::ptr base) {
    if (!base)
        base = $this(Class);
    if (superClass)
        return superClass->makeInstance(base);
    return std::make_shared<Instance>(base);
}

ObjPtr Class::call(ArgsList arguments, Interpreter *interpreter) {
    auto instance = makeInstance();

    auto init = instance->findAttribute("init");
    if (init)
        interpreter->call(init, arguments);

    return instance;
}

bool Class::checkArguments(int count) {
    return count == 0;
}

Class::Class(Class::ptr superclass) : superClass(superclass) {}