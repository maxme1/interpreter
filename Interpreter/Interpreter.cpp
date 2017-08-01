#include <iostream>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/Types/Int.h"
#include "../Object/native.h"


Interpreter::Interpreter() {
    addScope();
    setVariable("print", new Print());
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

void Interpreter::addScope() {
    scopes.push_back(new Object());
}

void Interpreter::deleteScope() {
    delete scopes.back();
    scopes.pop_back();
}

Object *Interpreter::getVariable(const std::string &name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); it++) {
        auto result = (*it)->attributes.find(name);
        if (result != (*it)->attributes.end())
            return result->second;
    }
    throw Exception("No variable named " + name);
}

void Interpreter::setVariable(const std::string &name, Object *value) {
    scopes.back()->setAttribute(name, value);
}

Object *Interpreter::track(Object *object) {
//    whenever the interpreter _might_ create a new object we store it
    object->mentions++;
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