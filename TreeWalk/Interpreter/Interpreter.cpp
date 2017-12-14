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

    auto sm = SemanticAnalyser(statements);
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

ObjPtr Interpreter::call(Callable::ptr callable, ArgsList arguments) {
//    ObjPtr returnObject = nullptr;
//    try {
    auto returnObject = callable->call(arguments, this);
//    } catch (ReturnException &e) {
//        returnObject = e.content;
//    } catch (FlowException &e) {
//        TODO: move to syntactic errors
//        throw Wrap(new SyntaxError("Control flow outside loop"));
//    }

    if (!returnObject)
        return New(None());
    return returnObject;
}

ObjPtr Interpreter::callOperator(ObjPtr object, ArgsList arguments) {
    auto callable = getCallable(std::move(object));
    checkArguments(callable, arguments.size());
    return call(callable, arguments);
}

std::vector<ObjPtr> Interpreter::evaluateArguments(const std::vector<Expression *> &argsList) {
    auto result = std::vector<ObjPtr>();
    for (auto &&argument : argsList)
        result.push_back(argument->visit(this));
    return result;
}

//bool Interpreter::isDerived(ObjPtr derived, Class::ptr base) {
//    if (base == nullptr)
//        return false;
//    auto instance = std::dynamic_pointer_cast<Instance>(derived);
//    Class::ptr theClass;
//    if (!instance)
//        theClass = std::dynamic_pointer_cast<Class>(derived);
//    else
//        theClass = instance->getClass();
//    if (!theClass)
//        throw Wrap(new ValueError("The object is not a class nor an instance"));
//    while (theClass != base and theClass != nullptr)
//        theClass = theClass->getSuperClass();
//    return theClass.get() == base.get();
//}

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

//Interpreter::ExceptionWrapper::ExceptionWrapper(const ObjPtr &exception) {
//    if (!isDerived(exception, Exception::build()))
//        throw Wrap(new ValueError("Only objects derived from Exception can be raised"));
//    this->exception = exception;
//}
