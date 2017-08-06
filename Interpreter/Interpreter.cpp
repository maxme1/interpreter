#include <iostream>
#include <utility>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/Native/Native.h"
#include "../Object/Types/None.h"
#include "../Object/Types/Array.h"
#include "../Object/Types/String.h"


Interpreter::Interpreter() {
    addScope();
    setVariable("print", new NativeFunction(
            [](ArgsList args) -> Object * {
                std::cout << args[0]->asString() << std::endl;
                return nullptr;
            }, 1));
    setVariable("Array", Array::build());
    setVariable("String", new StringClass());
}

Interpreter::~Interpreter() {
    deleteScope();
    collect();
}

void Interpreter::interpret(std::string text) {
    Tokenizer t = Tokenizer(std::move(text));
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
        std::cout << e.asString();
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

Object *Interpreter::call(Callable *callable, ArgsList arguments) {
    Object *returnObject = nullptr;
    try {
        returnObject = callable->__call__(arguments, this);
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
        auto arguments = std::vector<Object *>();
        for (auto &&argument : argsList) {
            arguments.push_back(argument->evaluate(this));
        }
        auto result = call(callable, arguments);
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

Object *Interpreter::callOperator(Object *object, ArgsList arguments) {
    auto *callable = getCallable(object);
    checkArguments(callable, arguments.size());

    addScope(callable->context);
    addScope();

    try {
        auto result = call(callable, arguments);
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
