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
    setVariable("print", New(NativeFunction($lambda {
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
    }, 0, true)));

    setVariable("Int", Int::build());
    setVariable("Array", Array::build());
    setVariable("String", String::build());
    setVariable("Bool", Bool::build());
    setVariable("Exception", Exception::build());
    setVariable("AttributeError", AttributeError::build());
}

Interpreter::~Interpreter() {
    deleteScope();
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
    auto end = scopes.rend() - 1;
    for (auto scope = scopes.rbegin(); scope != end; scope++) {
        auto result = (*scope)->findAttribute(name);
        if (result)
            return result;
    }
    return scopes[0]->getAttribute(name);
}

Scope::ptr Interpreter::getContext() {
    return scopes.back();
}

void Interpreter::setVariable(const std::string &name, ObjPtr value) {
    scopes.back()->setAttribute(name, value);
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

ObjPtr Interpreter::callOperator(ObjPtr object, ArgsList arguments) {
    auto callable = getCallable(object);
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

Interpreter::ExceptionWrapper::ExceptionWrapper(Object *exception) {
    if (!isDerived(ObjPtr(exception), Exception::build()))
        throw Wrap(new ValueError("Only objects derived from Exception can be raised"));
    this->exception = ObjPtr(exception);
}

Interpreter::ExceptionWrapper::ExceptionWrapper(const ObjPtr &exception) {
    if (!isDerived(exception, Exception::build()))
        throw Wrap(new ValueError("Only objects derived from Exception can be raised"));
    this->exception = exception;
}
