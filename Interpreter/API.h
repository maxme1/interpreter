#ifndef INTERPRETER_API_H
#define INTERPRETER_API_H

#include "../Object/Object.h"
#include "../Object/Callable.h"

class API {
    friend class Function;
    Interpreter *interpreter;
public:
    explicit API(Interpreter *interpreter) : interpreter(interpreter) {}

    Object *getVariable(const std::string &name) {
        return interpreter->getVariable(name);
    }

    void setVariable(const std::string &name, Object *value) {
        interpreter->setVariable(name, value);
    }

    Object *call(Object *object, const std::vector<Object *> &arguments) {
        return interpreter->callOperator(object, arguments);
    }
};

#endif //INTERPRETER_API_H
