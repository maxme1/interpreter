#include <iostream>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/native.h"
#include "../Object/Types/None.h"


Interpreter::Interpreter() {
    addScope();
    setVariable("print", new FromFunction(
            [](Object *args) -> Object * {
                auto arg = args->getAttribute("arg");
                std::cout << arg->str() << std::endl;
                return nullptr;
            }, {"arg"}));
}

Interpreter::~Interpreter() {
    deleteScope();
    collect();
}

void Interpreter::interpret(std::string text) {
    Tokenizer t = Tokenizer(text);
    auto tokens = t.tokenize();
    if (t.error) {
        //    TODO: it would be better to know the position in the text on error
        std::cout << "Undefined token";
        return;
    }

    Parser p = Parser(tokens);
    auto statements = p.build();
    if (p.error) {
        return;
    }
    try {
        evaluateStatements(statements);
    } catch (Exception &e) {
        std::cout << e.str();
    } catch (ControlFlow) {
        std::cout << "Control flow outside loop";
    } catch (ReturnException) {
        std::cout << "return outside function";
    }
    collect();
}

void Interpreter::addScope(Scope *ready) {
    Scope *lower;
    if (!ready)
        lower = new Scope();
    else
        lower = ready;
    if (scope)
        lower->setUpper(scope);
    scope = lower;
}

void Interpreter::deleteScope() {
    assert(scope);
    auto upper = scope->getUpper();
    if (scope->canDelete())
        delete scope;
    scope = upper;
}

Object *Interpreter::getVariable(const std::string &name) {
    scope->getAttribute(name);
}

void Interpreter::setVariable(const std::string &name, Object *value) {
    scope->setAttribute(name, value);
}

Object *Interpreter::track(Object *object) {
//    whenever the interpreter _might_ create a new object we store it
    object->save();
    garbage.push(object);
    return object;
}

void Interpreter::collect() {
    while (!garbage.empty()) {
        auto obj = garbage.top();
        garbage.pop();
        if (obj and obj->canDelete())
            delete obj;
    }
}

void Interpreter::evaluateStatements(std::vector<Statement *> &statements) {
    for (auto statement : statements) {
        statement->evaluate(this);
        collect();
    }
}

Callable *Interpreter::getCallable(Object *object) {
    auto *callable = dynamic_cast<Callable *>(object);
    if (callable)
        return callable;
    throw Exception("Object is not callable");
}

void Interpreter::checkArguments(Callable *callable, int count) {
    if (!callable->checkArguments(count))
        throw Exception("Number of arguments doesn't match");
}

Object *Interpreter::call(Callable *callable) {
    Object *returnObject = nullptr;
    try {
        returnObject = callable->__call__(scope, this);
    } catch (ReturnException &e) {
        returnObject = e.content;
    } catch (FlowException &e) {
//        TODO: move to syntactic errors
        throw Exception("Control flow outside loop");
    }
    if (!returnObject)
        returnObject = new None;
    return track(returnObject);
}

Object *Interpreter::callFunction(Object *object, const std::vector<Expression *> &argsList) {
    auto callable = getCallable(object);
    checkArguments(callable, argsList.size());

    addScope(callable->context);
    addScope();

    try {
        for (int i = 0; i < argsList.size(); i++) {
            auto arg = argsList[i]->evaluate(this);
            setVariable(callable->argument(i), arg);
        }
        auto result = call(callable);
        deleteScope();
        deleteScope();
        return result;
    }
    catch (Exception &e) {
        deleteScope();
        deleteScope();
        throw e;
    }
}

Object *Interpreter::callOperator(Object *object, std::initializer_list<Object *> arguments) {
    auto *callable = getCallable(object);
    checkArguments(callable, arguments.size());

    addScope(callable->context);
    addScope();

    try {
        int i = 0;
        for (auto &&argument : arguments) {
            setVariable(callable->argument(i), argument);
            i++;
        }
        auto result = call(callable);
        deleteScope();
        deleteScope();
        return result;
    }
    catch (Exception &e) {
        deleteScope();
        deleteScope();
        throw e;
    }
}
