#ifndef INTERPRETER_FUNCTION_H
#define INTERPRETER_FUNCTION_H

#include <utility>

#include "Object.h"
#include "Scope.h"

typedef const std::vector<ObjPtr> &ArgsList;
typedef const std::map<std::string, ObjPtr> &KwargsList;

class Interpreter;
struct Callable : public Object {
    friend class Interpreter;
    friend class ClassMethod;
    Scope::ptr closure = nullptr;
protected:
    virtual void checkArguments(ArgsList positional, KwargsList keyword) = 0;
    virtual ObjPtr call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) = 0;
public:
    Callable();
    explicit Callable(Scope::ptr closure);

    typedef std::shared_ptr<Callable> ptr;
};

class FunctionPrototype : public Callable {
    friend class Interpreter;

public:
    struct Argument {
        std::string name;
        ObjPtr defaultValue;
        bool positional, variable;

        Argument(const std::string &name, ObjPtr defaultValue, bool positional, bool variable) :
                name(name), defaultValue(defaultValue), positional(positional), variable(variable) {}
    };
    explicit FunctionPrototype(std::vector<Argument> &arguments);
protected:
    std::vector<Argument> arguments;
    int required;
    bool variable;
    void checkArguments(ArgsList positional, KwargsList keyword) override;
};

class Statement;
class Function : public FunctionPrototype {
    friend class Interpreter;
    shared(Statement) body = nullptr;

protected:
    ObjPtr call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) override;

public:
    explicit Function(shared(Statement) body, Scope::ptr closure, std::vector<FunctionPrototype::Argument> &arguments);
};

class Instance;
class ClassMethod : public Callable {
    friend class Interpreter;
    Callable::ptr function;
    std::shared_ptr<Instance> instance;
protected:
    void checkArguments(ArgsList positional, KwargsList keyword) override;
    ObjPtr call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) override;

public:
    ClassMethod(Callable::ptr function, std::shared_ptr<Instance> instance);
};

#endif //INTERPRETER_FUNCTION_H
