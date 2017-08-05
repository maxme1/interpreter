#include "Class.h"

// Instance

ClassInstance::ClassInstance(Object *classPtr) : classPtr(classPtr) {
    classPtr->save();
}

ClassInstance::~ClassInstance() {
    if (classPtr->canDelete())
        delete classPtr;
}

Object *ClassInstance::findAttribute(const std::string &name) {
    auto result = Object::findAttribute(name);
    if (!result)
        result = classPtr->findAttribute(name);
    if (!result)
        return nullptr;
//        creating a class method
    auto method = dynamic_cast<Callable *> (result);
    if (method)
        return new ClassMethod(this, method);
    return result;
}

// Class

bool Class::checkArguments(int count) { return count == 0; }

Object *Class::__call__(ArgsList args, Interpreter *interpreter) {
    return new ClassInstance(this);
}

Class::Class(Scope *context) : Callable(context) {
//        copy the class's scope
    attributes = context->attributes;
    for (auto &&attribute : attributes) {
        attribute.second->save();
    }
}