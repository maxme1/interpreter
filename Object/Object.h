#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include <string>
#include <map>
#include <vector>

class Object {
    int mentions = 0;
    bool canDelete();
protected:
    std::map<std::string, Object *> attributes;
public:
    virtual ~Object();

    static void remove(Object *object);
    void save();

    virtual void setAttribute(const std::string &name, Object *value) final;
    virtual Object *getAttribute(const std::string &name) final;
    virtual Object *findAttribute(const std::string &name);

    virtual std::string asString() { return "Object"; };

    virtual bool asBool() { return false; };
};

#endif //INTERPRETER_OBJECT_H
