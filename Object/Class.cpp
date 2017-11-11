//#include "Class.h"
//
//#include "Native/NativeObject.h"
//
//// Instance
//
//std::string Instance::asString() {
//    return "<" + getClass()->asString() + " instance>";
//}
//
//ObjPtr Instance::findAttribute(const std::string &name) {
//    auto result = Object::findAttribute(name);
//    if (!result)
//        result = getClass()->findAttribute(name);
//    if (!result)
//        return nullptr;
////        creating a class method
//    auto method = std::dynamic_pointer_cast<Callable>(result);
//    if (method)
//        return New(ClassMethod(method, $this(Instance)));
//    return result;
//}
//
//Instance::Instance(Class::ptr classPtr) : classPtr(std::move(classPtr)) {}
//
//Class::ptr Instance::getClass() {
//    assert(classPtr);
//    return classPtr;
//}
//
//// Class
//
//Instance::ptr Class::makeInstance(const ptr &instanceClass) {
//    if (instanceClass)
//        return std::make_shared<Instance>(instanceClass);
//    auto theClass = $this(Class);
//    while (theClass->superclass)
//        theClass = theClass->superclass;
//    return theClass->makeInstance($this(Class));
//}
//
//Class::Class(const std::string &name, Scope::ptr body, Class::ptr superclass, Scope::ptr context) :
//        superclass(std::move(superclass)), context(std::move(context)) {
////    copy the scope
//    for (auto &&attribute : body->attributes)
//        setAttribute(attribute.first, attribute.second);
//}
//
//ObjPtr Class::findAttribute(const std::string &name) {
//    auto result = Object::findAttribute(name);
//    if (!result and superclass)
//        return superclass->findAttribute(name);
//    return result;
//}
//
//Class::ptr Class::getSuperClass() {
//    return superclass;
//}
//
//std::string Class::asString() {
//    auto name = findAttribute("@name");
//    std::string result = "<class";
//    if (name)
//        result += " " + name->asString();
//    return result + ">";
//}
//
//Class::Class(Class::ptr superclass) : superclass(std::move(superclass)) {}