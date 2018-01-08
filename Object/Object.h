#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include <string>
#include <map>
#include <vector>
#include <memory>

#define shared(type) std::shared_ptr<type>

class Object : public std::enable_shared_from_this<Object> {
    std::map<std::string, std::shared_ptr<Object>> attributes;
public:
    Object() = default;
    virtual void setAttribute(const std::string &name, shared(Object) value) final;
    virtual shared(Object) getAttribute(const std::string &name) final;
    virtual shared(Object) findAttribute(const std::string &name);

    virtual bool asBool() { return false; };
    virtual std::string asString() { return "NO STR METHOD"; };
};

typedef shared(Object) ObjPtr;
#endif //INTERPRETER_OBJECT_H
