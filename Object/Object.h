#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include <string>
#include <map>
#include <vector>

class Object {
    int mentions = 0;
protected:
public:
    virtual ~Object();

    std::map<std::string, Object *> attributes;
    bool canDelete();

    void save() { mentions++; }

    virtual std::string str() { return "Object"; };

    void setAttribute(const std::string &name, Object *value);
    virtual Object *getAttribute(const std::string &name);
    virtual Object * findAttribute(const std::string &name);

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
