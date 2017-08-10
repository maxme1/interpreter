#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include <string>
#include <map>
#include <vector>
#include <bits/shared_ptr.h>

class Object;
typedef std::shared_ptr<Object> ObjPtr;
class Object {
protected:
    std::map<std::string, ObjPtr> attributes;
public:
    virtual void setAttribute(const std::string &name, ObjPtr value) final;
    virtual ObjPtr getAttribute(const std::string &name) final;
    virtual ObjPtr findAttribute(const std::string &name);

    virtual std::string asString() { return "Object"; };

    virtual bool asBool() { return false; };
};

#endif //INTERPRETER_OBJECT_H
