#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include <string>
#include <map>

struct Object {
    std::map<std::string, Object *> attributes;

    virtual std::string str() { return "Object"; };

    void setAttribute(std::string name, Object *value);
    Object *getAttribute(std::string name);

    virtual Object *add(Object *other) { return nullptr; };

    virtual Object *uadd() { return nullptr; };

    virtual Object *sub(Object *other) { return nullptr; };

    virtual Object *usub() { return nullptr; };

    virtual Object *mul(Object *other) { return nullptr; };

    virtual Object *div(Object *other) { return nullptr; };
};

struct Number : public Object {
    int value;

    std::string str() override;

    Object *add(Object *other) override;
    Object *uadd() override;

    Object *sub(Object *other) override;
    Object *usub() override;

    Object *mul(Object *other) override;

    Object *div(Object *other) override;

    Number(int value);
};


#endif //INTERPRETER_OBJECT_H
