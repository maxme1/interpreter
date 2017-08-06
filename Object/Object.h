#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include <string>
#include <map>
#include <vector>

class Object {
    int mentions = 0;
protected:
    std::map<std::string, Object *> attributes;
public:
    virtual ~Object();

    bool canDelete();

    void save() { mentions++; }

    void setAttribute(const std::string &name, Object *value);
    virtual Object *getAttribute(const std::string &name);
    virtual Object *findAttribute(const std::string &name);

    virtual std::string asString() { return "Object"; };
    virtual bool asBool() { return false; };
};

#endif //INTERPRETER_OBJECT_H
