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

    virtual ~Object();

    std::map<std::string, Object *> attributes;

    virtual std::string str() { return "Object"; };

    void setAttribute(const std::string &name, Object *value);
    Object *getAttribute(const std::string &name);

    virtual Object *add(Object *other) { return nullptr; };

    virtual Object *unary_add() { return nullptr; };

    virtual Object *subtract(Object *other) { return nullptr; };

    virtual Object *unary_subtract() { return nullptr; };

    virtual Object *multiply(Object *other) { return nullptr; };

    virtual Object *divide(Object *other) { return nullptr; };

    virtual Object *equal(Object *other) { return nullptr; };

    virtual Object *greater(Object *other) { return nullptr; };

    virtual Object *less(Object *other) { return nullptr; };

    virtual Object *greater_or_equal(Object *other) { return nullptr; };

    virtual Object *less_or_equal(Object *other) { return nullptr; };

    virtual Object *not_equal(Object *other) { return nullptr; };

    virtual Object *__call__(Object *args) { throw "Object is not callable"; };

    virtual Bool *__bool__();
};

#endif //INTERPRETER_OBJECT_H
