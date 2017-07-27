#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include <string>
#include <map>

// TODO: throw 'Operator not defined here'
class Bool;
class Object {
    int mentions = 0;

    bool canDelete();
public:
    virtual bool asBool() { return false; };
    bool zombie();

    virtual ~Object();

    std::map<std::string, Object *> attributes;

    virtual std::string str() { return "Object"; };

    void setAttribute(std::string name, Object *value);
    Object *getAttribute(std::string name);

    virtual Object *add(Object *other) { return nullptr; };

    virtual Object *uadd() { return nullptr; };

    virtual Object *sub(Object *other) { return nullptr; };

    virtual Object *usub() { return nullptr; };

    virtual Object *mul(Object *other) { return nullptr; };

    virtual Object *div(Object *other) { return nullptr; };

    virtual Bool *__bool__();
};

#endif //INTERPRETER_OBJECT_H
