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
    assert(result);
//     creating a class method
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
        Callable(std::move(closure)), superclass(std::move(superclass)) {
//    copy the scope
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
    return "<class>";
}

ObjPtr Class::call(const std::vector<ObjPtr> &args, Interpreter *interpreter) {
    auto instance = std::make_shared<Instance>($this(Class));
    return instance;

//    ObjPtr init;
//    addScope(classObject->context);
//    try {
//        init = instance->findAttribute("init");
//        if (init)
//            callFunction(init, expression->argsList);
//    } catch (ExceptionWrapper &e) {
//        deleteScope();
//        throw;
//    }
//    deleteScope();
//    if (!init and !expression->argsList.empty())
//        throw Wrap(new Exception("Default constructor does not receive arguments"));

}

bool Class::checkArguments(int count) {
    return count == 0;
}

//Class::Class(Class::ptr superclass) : superclass(std::move(superclass)) {}