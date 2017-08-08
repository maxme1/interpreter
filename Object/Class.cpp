#include "Class.h"
#include "Native/NativeObject.h"

// Instance

Instance::Instance(Class *classPtr) : classPtr(classPtr) {
    classPtr->save();
}

Instance::~Instance() {
    if (classPtr->canDelete())
        delete classPtr;
}

Object *Instance::findAttribute(const std::string &name) {
    auto result = Object::findAttribute(name);
    if (!result and classPtr)
        result = classPtr->findAttribute(name);
    if (!result)
        return nullptr;
//        creating a class method
    auto method = dynamic_cast<Callable *> (result);
    if (method)
        return new ClassMethod(this, method);
    return result;
}

Object *Instance::getSuperClass() {
    if (classPtr and classPtr->superclass)
        return classPtr->superclass;
    return nullptr;
}

// Class

bool Class::checkArguments(int count) { return count == 0; }

Object *Class::__call__(ArgsList args, API *api) {
    return new Instance(this);
}

Class::Class(Object *context, Class *superclass) : Callable(context) {
    if (!superclass) {
        this->superclass = nullptr;
        return;
    }
    auto native = dynamic_cast<NativeClass *>(superclass);
    if (native) {
        this->superclass = native->__call__({}, nullptr);
    } else
        this->superclass = superclass;
    this->superclass->save();
}

Class::~Class() {
    if (superclass and superclass->canDelete())
        delete superclass;
}

Object *Class::findAttribute(const std::string &name) {
    auto result = Object::findAttribute(name);
    if (!result and superclass)
        return superclass->findAttribute(name);
    return result;
}
