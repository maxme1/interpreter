#include <iostream>
#include <utility>
#include <fstream>
#include "Interpreter.h"
#include "../../Tokenizer/Tokenizer.h"
#include "../../Parser/Parser.h"
#include "../../Object/Native/Native.h"
#include "../../Object/Types/Int.h"
#include "../../Object/Types/None.h"
#include "../Semantics/SemanticAnalyser.h"

Interpreter::Interpreter() {
//    api = new API(this);
    enterScope();
    populate();
}

Interpreter::~Interpreter() {
//    deleteScope();
//    delete api;
}

void Interpreter::interpret(std::string text) {
    Tokenizer t = Tokenizer(text);
    auto tokens = t.tokenize();
    if (t.error) {
        auto token = tokens.back();
        printf("Unrecognized token %s at %li:%li", token.body.c_str(), token.line, token.column);
        return;
    }
    Parser p = Parser(tokens);
    auto statements = p.build();
    if (p.error) {
        std::cout << "\n==========\n" << p.message;
        return;
    }

    try {
        auto sm = SemanticAnalyser(statements);
    } catch (SemanticAnalyser::SyntaxError &e) {
        std::cout << "\n==========\n" << e.message;
        return;
    }

//
//    try {
    visitStatements(statements);
//    } catch (Wrap &e) {
//        std::cout << "==========\n" << e.exception->asString();
//    }
}

void Interpreter::enterScope() {
    Scope::ptr newScope = std::make_shared<Scope>();
    if (!scopes.empty())
        newScope->setUpper(scopes.back());
    scopes.push_back(newScope);
}

void Interpreter::addScope(std::shared_ptr<Scope> scope) {
    assert(scope);
    scopes.push_back(scope);
}

void Interpreter::leaveScope() {
    assert(!scopes.empty());
    scopes.pop_back();
}

Scope::ptr Interpreter::getClosure() {
    assert(!scopes.empty());
    return scopes.back();
}

ObjPtr Interpreter::getVariable(const std::string &name, long level) {
    assert(!scopes.empty());
    auto result = scopes.back()->findAttribute(name, level);
    if (!result)
        printf(name.c_str());
    assert(result);
    return result;
//    throw Wrap(new VariableError(name));
}

void Interpreter::setVariable(const std::string &name, ObjPtr value, long level) {
    assert(!scopes.empty());
    scopes.back()->setAttribute(name, std::move(value), level);
}

void Interpreter::defineVariable(const std::string &name, ObjPtr value) {
    assert(!scopes.empty());
    scopes.back()->defineVariable(name, std::move(value));
}

Callable::ptr Interpreter::getCallable(ObjPtr object) {
    auto callable = std::dynamic_pointer_cast<Callable>(object);
    assert(callable);
    return callable;
//    throw Wrap(new ValueError("Object is not callable"));
}

void Interpreter::checkArguments(Callable::ptr callable, long count) {
//    if (!callable->checkArguments(count))
//        throw Wrap(new ValueError("Number of arguments doesn't match: " + std::to_string(count)));
}

ObjPtr Interpreter::call(ObjPtr object, ArgsList arguments) {
    auto callable = getCallable(object);
    if (callable->closure)
        addScope(callable->closure);
    else
        enterScope();

    ObjPtr returnObject;
    try {
        returnObject = callable->call(arguments, this);
    } catch (ReturnException &e) {
        returnObject = e.content;
    } catch (ExceptionWrapper &e) {
        leaveScope();
        throw;
    }
    leaveScope();

    if (!returnObject)
        return New(None());
    return returnObject;
}

ObjPtr Interpreter::callOperator(ObjPtr object, ArgsList arguments) {
    auto callable = getCallable(object);
    checkArguments(callable, arguments.size());
    return call(callable, arguments);
}

std::vector<ObjPtr> Interpreter::evaluateArguments(const std::vector<Expression *> &argsList) {
    auto result = std::vector<ObjPtr>();
    for (auto &&argument : argsList)
        result.push_back(argument->visit(this));
    return result;
}

bool Interpreter::isDerived(ObjPtr derived, ObjPtr base) {
    assert(base and derived);
    auto derivedClass = std::dynamic_pointer_cast<Class>(derived);
    assert(derivedClass);
    auto baseClass = std::dynamic_pointer_cast<Class>(base);
    assert(baseClass);

    while (derivedClass != nullptr) {
        if (derivedClass == baseClass)
            return true;
        derivedClass = derivedClass->superClass;
    }
    return false;
}

bool Interpreter::isInstance(ObjPtr instance, ObjPtr base) {
    assert(base and instance);
    auto instance_ = std::dynamic_pointer_cast<Instance>(instance);
    assert(instance_);
    auto baseClass = std::dynamic_pointer_cast<Class>(base);
    assert(baseClass);

    return isDerived(instance_->getClass(), baseClass);
}

bool Interpreter::interpretFile(const std::string &path) {
    std::ifstream source(path);
    if (!source.good())
        return false;
    std::string text((std::istreambuf_iterator<char>(source)),
                     (std::istreambuf_iterator<char>()));
    interpret(text);
    source.close();
    return true;
}

//Interpreter::ExceptionWrapper::ExceptionWrapper(Object *exception) : ExceptionWrapper(ObjPtr(exception)) {}

Interpreter::ExceptionWrapper::ExceptionWrapper(ObjPtr exception) {
    if (std::dynamic_pointer_cast<Instance>(exception))
        assert(isInstance(exception, Exception::build()));
    else
        assert(isDerived(exception, Exception::build()));
//        throw Wrap(new ValueError("Only objects derived from Exception can be raised"));
    this->exception = exception;
}
