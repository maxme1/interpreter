#include <iostream>
#include <utility>
#include <fstream>
#include "Interpreter.h"
#include "../../Tokenizer/Tokenizer.h"
#include "../../Parser/Parser.h"
#include "../../Object/Native/Native.h"
#include "../../Object/Types/Int.h"
#include "../../Object/Exception.h"
#include "../../Object/Types/None.h"
#include "../Semantics/SemanticAnalyser.h"

Interpreter::Interpreter() {
    enterScope();
    populate();
}

Interpreter::~Interpreter() {
    while (!scopes.empty())
        leaveScope();
}

void Interpreter::interpret(std::string text) {
    try {
        _interpret(text);
    } catch (BaseExceptionWrapper &e) {
        std::cout << "==========\n" << e.message;
        if (!e.traceback.empty())
            printf(" at %li:%li", e.traceback[0].line, e.traceback[0].column);
    }
}

void Interpreter::_interpret(std::string text) {
    Tokenizer t = Tokenizer(text);
    auto tokens = t.tokenize();
    Parser p = Parser(tokens);
    auto statements = p.getStatements();
    auto sm = SemanticAnalyser(statements);
    visitStatements(statements);
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
        throw ExceptionWrapper(new VariableError("Variable " + name + " is not declared in this scope"));
    return result;
}

void Interpreter::setVariable(const std::string &name, ObjPtr value, long level) {
    assert(!scopes.empty());
    if (!scopes.back()->setAttribute(name, std::move(value), level))
        throw ExceptionWrapper(new VariableError("Variable " + name + " is not declared in this scope"));
}

void Interpreter::defineVariable(const std::string &name, ObjPtr value) {
    assert(!scopes.empty());
    scopes.back()->defineVariable(name, std::move(value));
}

Callable::ptr Interpreter::getCallable(ObjPtr object) {
    auto callable = std::dynamic_pointer_cast<Callable>(object);
    if (!callable)
        throw ExceptionWrapper(new ValueError("Object is not callable"));
    return callable;
}

ObjPtr Interpreter::call(ObjPtr object, ArgsList positional, KwargsList keyword) {
    auto callable = getCallable(object);
    callable->checkArguments(positional, keyword);

    if (callable->closure)
        addScope(callable->closure);
    else
        enterScope();

    ObjPtr returnObject;
    try {
        returnObject = callable->call(this, positional, keyword);
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

std::vector<ObjPtr> Interpreter::evaluateArguments(const std::vector<Expression::ptr> &argsList) {
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

Interpreter::ExceptionWrapper::ExceptionWrapper(Object *exception) : ExceptionWrapper(ObjPtr(exception)) {}

Interpreter::ExceptionWrapper::ExceptionWrapper(ObjPtr exception) {
    auto instance = std::dynamic_pointer_cast<Instance>(exception);
    if (instance == nullptr or not isInstance(exception, Exception::build()))
        throw ExceptionWrapper(new ValueError("Only instances derived from Exception can be raised"));
    this->exception = instance;
    message = String::cast(this->exception->getAttribute("body"))->string;
}
