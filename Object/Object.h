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
public:
    virtual ~Object();

    std::map<std::string, Object *> attributes;

    virtual std::string str() { return "Object"; };

    void setAttribute(const std::string &name, Object *value);
    Object *getAttribute(const std::string &name);

    virtual Object *add(Object *other);
    virtual Object *unary_add();
    virtual Object *subtract(Object *other);
    virtual Object *unary_subtract();
    virtual Object *multiply(Object *other);
    virtual Object *divide(Object *other);
    virtual Object *equal(Object *other);
    virtual Object *greater(Object *other);
    virtual Object *less(Object *other);
    virtual Object *greater_or_equal(Object *other);
    virtual Object *less_or_equal(Object *other);
    virtual Object *not_equal(Object *other);

    virtual bool asBool() { return false; };
};

#endif //INTERPRETER_OBJECT_H
