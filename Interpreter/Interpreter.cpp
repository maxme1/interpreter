#include <iostream>
#include <utility>
#include <fstream>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/Native/Native.h"
#include "../Object/Types/Int.h"
#include "../Object/Types/None.h"

Interpreter::Interpreter() {
    api = new API(this);
    addScope();
    populate();
}

Interpreter::~Interpreter() {
    deleteScope();
    delete api;
}

void Interpreter::interpret(std::string text) {
    Tokenizer t = Tokenizer(text);
    auto tokens = t.tokenize();
//    TODO: now it's just a mess
    if (t.error) {
        int position = tokens.back().position;
        int begin = std::max<int>(0, position - textRange);
        std::cout << text.substr(begin, textRange);
        std::cout << " <<<\n==========\nUndefined token: " << tokens.back().body;
        return;
    }

    Parser p = Parser(tokens);
    auto statements = p.build();
    if (p.error) {
        int position = p.position->position;
        int begin = std::max<int>(0, position - textRange);
        std::cout << text.substr(begin, textRange);
        std::cout << " <<<\n==========\n" << p.message;
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
}

void Interpreter::addScope(Scope::ptr context) {
    Scope::ptr newScope;
    if (context)
        newScope = context;
    else {
        newScope = std::make_shared<Scope>();
        if (!scopes.empty())
            newScope->setUpper(scopes.back());
    }
    scopes.push_back(newScope);
}

void Interpreter::deleteScope() {
    assert(!scopes.empty());
    scopes.pop_back();
}

ObjPtr Interpreter::getVariable(const std::string &name) {
    assert(!scopes.empty());
    for (auto scope = scopes.rbegin(); scope != scopes.rend(); scope++) {
        auto result = (*scope)->findAttribute(name);
        if (result)
            return result;
    }
    throw Wrap(new VariableError(name));
}

Scope::ptr Interpreter::getContext() {
    return scopes.back();
}

void Interpreter::setVariable(const std::string &name, ObjPtr value) {
    scopes.back()->setAttribute(name, std::move(value));
}

void Interpreter::evaluateStatements(std::vector<Statement *> &statements) {
    for (auto statement : statements)
        statement->evaluate(this);
}

Callable::ptr Interpreter::getCallable(ObjPtr object) {
    auto callable = std::dynamic_pointer_cast<Callable>(object);
    if (callable)
        return callable;
    throw Wrap(new ValueError("Object is not callable"));
}

void Interpreter::checkArguments(Callable::ptr callable, int count) {
    if (!callable->checkArguments(count))
        throw Wrap(new ValueError("Number of arguments doesn't match: " + std::to_string(count)));
}

ObjPtr Interpreter::call(Callable::ptr callable, ArgsList arguments) {
    ObjPtr returnObject = nullptr;
    try {
        returnObject = callable->call(arguments, api);
    } catch (ReturnException &e) {
        returnObject = e.content;
    } catch (FlowException &e) {
//        TODO: move to syntactic errors
        throw Wrap(new SyntaxError("Control flow outside loop"));
    }
    if (!returnObject)
        returnObject = New(None());
    return returnObject;
}

ObjPtr Interpreter::callFunction(ObjPtr object, const std::vector<Expression *> &argsList) {
    auto callable = getCallable(std::move(object));
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

ObjPtr Interpreter::callOperator(ObjPtr object, ArgsList arguments) {
    auto callable = getCallable(std::move(object));
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

std::vector<ObjPtr> Interpreter::evaluateArguments(const std::vector<Expression *> &argsList) {
    auto result = std::vector<ObjPtr>();
    for (auto &&argument : argsList)
        result.push_back(argument->evaluate(this));
    return result;
}

bool Interpreter::isDerived(ObjPtr derived, Class::ptr base) {
    if (base == nullptr)
        return false;
    auto instance = std::dynamic_pointer_cast<Instance>(derived);
    Class::ptr theClass;
    if (!instance)
        theClass = std::dynamic_pointer_cast<Class>(derived);
    else
        theClass = instance->getClass();
    if (!theClass)
        throw Wrap(new ValueError("The object is not a class nor an instance"));
    while (theClass != base and theClass != nullptr)
        theClass = theClass->getSuperClass();
    return theClass.get() == base.get();
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

Interpreter::ExceptionWrapper::ExceptionWrapper(const ObjPtr &exception) {
    if (!isDerived(exception, Exception::build()))
        throw Wrap(new ValueError("Only objects derived from Exception can be raised"));
    this->exception = exception;
}
