#include <iostream>
#include <utility>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/Native/Native.h"
#include "../Object/Types/Int.h"
#include "../Object/Types/Array.h"
#include "../Object/Types/None.h"

Interpreter::Interpreter() {
    api = new API(this);
    addScope();
    setVariable("print", new NativeFunction($lambda {
        bool first = true;
        for (auto &&arg : args) {
            if (not first) {
                std::cout << " ";
            } else
                first = false;
            std::cout << String::toString({arg}, api);
        }
        std::cout << std::endl;
        return nullptr;
    }, 0, true));

    setVariable("Int", Int::build());
    setVariable("Array", Array::build());
    setVariable("String", String::build());
    setVariable("Bool", Bool::build());
    setVariable("Exception", Exception::build());
    setVariable("AttributeError", AttributeError::build());
}

Interpreter::~Interpreter() {
    deleteScope();
    collect();
    delete api;
}

void Interpreter::interpret(std::string text) {
    Tokenizer t = Tokenizer(std::move(text));
    auto tokens = t.tokenize();
    if (t.error) {
        //    TODO: it would be better to know the position in the text on error
        std::cout << "Undefined token:" << tokens.back().body;
        return;
    }

    Parser p = Parser(tokens);
    auto statements = p.build();
    if (p.error) {
        return;
    }
    try {
        evaluateStatements(statements);
    } catch (Wrap &e) {
        std::cout << "==========\n" << e.exception->asString();
    } catch (ControlFlow) {
        std::cout << "Control flow outside loop";
    } catch (ReturnException) {
        std::cout << "return outside function";
    }
    collect();
}

void Interpreter::addScope(Scope *context) {
    Scope *newScope;
    if (context)
        newScope = context;
    else {
        newScope = new Scope();
        if (!scopes.empty())
            newScope->setUpper(scopes.back());
    }
    track(newScope);
    scopes.push_back(newScope);
}

void Interpreter::deleteScope() {
    assert(!scopes.empty());
    Object::remove(scopes.back());
    scopes.pop_back();
}

Object *Interpreter::getVariable(const std::string &name) {
    assert(!scopes.empty());
    auto end = scopes.rend() - 1;
    for (auto scope = scopes.rbegin(); scope != end; scope++) {
        auto result = (*scope)->findAttribute(name);
        if (result)
            return result;
    }
    return scopes[0]->getAttribute(name);
}

Scope *Interpreter::getContext() {
    return scopes.back();
}

void Interpreter::setVariable(const std::string &name, Object *value) {
    scopes.back()->setAttribute(name, value);
}

Object *Interpreter::track(Object *object) {
    assert(object);
//    whenever the interpreter _might_ create a new object we store it
    object->save();
    garbage.push(object);
    return object;
}

void Interpreter::collect() {
    while (!garbage.empty()) {
        auto obj = garbage.top();
        garbage.pop();
        Object::remove(obj);
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
    throw Wrap(new ValueError("Object is not callable"));
}

void Interpreter::checkArguments(Callable *callable, int count) {
    if (!callable->checkArguments(count))
        throw Wrap(new ValueError("Number of arguments doesn't match: " + std::to_string(count)));
}

Object *Interpreter::call(Callable *callable, ArgsList arguments) {
    Object *returnObject = nullptr;
    try {
        returnObject = callable->call(arguments, api);
    } catch (ReturnException &e) {
        returnObject = e.content;
    } catch (FlowException &e) {
//        TODO: move to syntactic errors
        throw Wrap(new SyntaxError("Control flow outside loop"));
    }
    if (!returnObject)
        returnObject = new None();
    return returnObject;
}

Object *Interpreter::callFunction(Object *object, const std::vector<Expression *> &argsList) {
    auto callable = getCallable(object);
    checkArguments(callable, argsList.size());

//    TODO: create a single function
    addScope(callable->context);
    addScope();

    try {
        auto arguments = evaluateArguments(argsList);
        auto result = call(callable, arguments);
        deleteScope();
        deleteScope();
        return result;
    }
    catch (ExceptionWrapper &e) {
        deleteScope();
        deleteScope();
        throw;
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
    catch (ExceptionWrapper &e) {
        deleteScope();
        deleteScope();
        throw;
    }
}

std::vector<Object *> Interpreter::evaluateArguments(const std::vector<Expression *> &argsList) {
    auto result = std::vector<Object *>();
    for (auto &&argument : argsList)
        result.push_back(track(argument->evaluate(this)));
    return result;
}

bool Interpreter::isDerived(Object *derived, Class *base) {
    if (base == nullptr)
        return false;
    auto instance = dynamic_cast<Instance *>(derived);
    Class *theClass;
    if (!instance)
        theClass = dynamic_cast<Class *>(derived);
    else
        theClass = instance->getClass();
    if (!theClass)
        throw Wrap(new ValueError("The object is not a class nor an instance"));
    while (theClass != base and theClass != nullptr)
        theClass = theClass->getSuperClass();
    return theClass == base;
}

Interpreter::ExceptionWrapper::ExceptionWrapper(Object *exception) {
    if (!isDerived(exception, Exception::build()))
        throw Wrap(new ValueError("Only objects derived from Exception can be raised"));
    this->exception = exception;
    exception->save();
}

Interpreter::ExceptionWrapper::~ExceptionWrapper() {
    Object::remove(exception);
}
