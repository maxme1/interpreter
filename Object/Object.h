#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include <string>
#include <map>
#include <vector>
#include <memory>

class Object;
typedef std::shared_ptr<Object> ObjPtr;
class Object : public std::enable_shared_from_this<Object> {
protected:
    std::map<std::string, ObjPtr> attributes;
public:
    virtual void setAttribute(const std::string &name, ObjPtr value) final;
    virtual ObjPtr getAttribute(const std::string &name) final;
    virtual ObjPtr findAttribute(const std::string &name);

    virtual std::string asString() { return "Object"; };

    virtual bool asBool() { return false; };

    typedef std::shared_ptr<Object> ptr;
};

#endif //INTERPRETER_OBJECT_H
