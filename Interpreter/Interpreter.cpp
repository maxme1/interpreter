#include <iostream>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/Types/Int.h"
#include "../Object/native.h"


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

void Interpreter::addScope() {
    auto lower = new Scope();
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