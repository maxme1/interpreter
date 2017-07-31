#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include <string>
#include <map>
#include <vector>
#include "../Parser/Statement/Statement.h"

// TODO: throw 'Operator not defined here'
class Bool;
class Object {
    friend class Interpreter;
    int mentions = 0;
    bool canDelete();
protected:
//    TODO: think of a better way
    Statement *functionBody = nullptr;
    std::vector<std::string> functionArguments;
public:
//    TODO: replace bool  with asBool
    virtual bool asBool() { return false; };
    bool zombie();

    virtual ~Object();

    std::map<std::string, Object *> attributes;

    virtual std::string str() { return "Object"; };

    void setAttribute(const std::string &name, Object *value);
    Object *getAttribute(const std::string &name);

    virtual Object *add(Object *other) { return nullptr; };

    virtual Object *uadd() { return nullptr; };

    virtual Object *sub(Object *other) { return nullptr; };

    virtual Object *usub() { return nullptr; };

    virtual Object *mul(Object *other) { return nullptr; };

    virtual Object *div(Object *other) { return nullptr; };

//    TODO: probably add a function to call from interpreter
    virtual Object *__call__(Object *args) { throw "Object is not callable"; };

    virtual Bool *__bool__();
};

#endif //INTERPRETER_OBJECT_H
