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
    virtual bool checkArguments(ArgsList positional, KwargsList keyword) = 0;
    virtual ObjPtr call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) = 0;
public:
    Callable();
    explicit Callable(Scope::ptr closure);

    typedef std::shared_ptr<Callable> ptr;
};

class Statement;
class Function : public Callable {
    friend class Interpreter;
    Statement *body = nullptr;
    std::vector<std::string> arguments;
    std::map<std::string, ObjPtr> defaults;

protected:
    bool checkArguments(ArgsList positional, KwargsList keyword) override ;
    ObjPtr call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) override;

public:
    explicit Function(Statement *body, Scope::ptr closure, std::vector<std::string> &arguments,
                      std::map<std::string, ObjPtr> defaults);
};

class Instance;
class ClassMethod : public Callable {
    friend class Interpreter;
    Callable::ptr function;
    std::shared_ptr<Instance> instance;
protected:
    bool checkArguments(ArgsList positional, KwargsList keyword) override ;
    ObjPtr call(Interpreter *interpreter, ArgsList positional, KwargsList keyword) override;

public:
    ClassMethod(Callable::ptr function, std::shared_ptr<Instance> instance);
};

#endif //INTERPRETER_FUNCTION_H
