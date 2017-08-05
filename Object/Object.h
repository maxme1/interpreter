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
    virtual Object *findAttribute(const std::string &name);

    virtual bool asBool() { return false; };
};

#endif //INTERPRETER_OBJECT_H
