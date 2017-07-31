#include <iostream>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/Types/Int.h"
#include "../Object/native.h"
#include "../Object/Types/Function.h"

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
//    std::cout << statements[0]->str();
    try {
        evaluateStatements(statements);
    } catch (const char *exception) {
        std::cout << exception;
    } catch (Token::tokenType type) {
        std::cout << "Control flow outside loop";
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
    throw "No variable named " + name;
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

Object *Interpreter::evaluate(Binary *expression) {
    Object *left = expression->left->evaluate(this), *right = expression->right->evaluate(this), *result;
    if (expression->type == Token::ADD)
        return track(left->add(right));
    if (expression->type == Token::SUB)
        return track(left->subtract(right));
    if (expression->type == Token::MUL)
        return track(left->multiply(right));
    if (expression->type == Token::DIV)
        return track(left->divide(right));
//    comparison
    if (expression->type == Token::EQUAL)
        return track(left->equal(right));
    if (expression->type == Token::NOT_EQUAL)
        return track(left->not_equal(right));
    if (expression->type == Token::GREATER_OR_EQUAL)
        return track(left->greater_or_equal(right));
    if (expression->type == Token::GREATER)
        return track(left->greater(right));
    if (expression->type == Token::LESS)
        return track(left->less(right));
    if (expression->type == Token::LESS_OR_EQUAL)
        return track(left->less_or_equal(right));
    return nullptr;
}

Object *Interpreter::evaluate(Unary *expression) {
    Object *argument = expression->argument->evaluate(this);
    if (expression->type == Token::ADD)
        return track(argument->unary_add());
    if (expression->type == Token::SUB)
        return track(argument->unary_subtract());
    if (expression->type == Token::BRACKET)
        return argument;
    return nullptr;
}

Object *Interpreter::evaluate(Literal *expression) {
    auto body = expression->body;
    auto type = expression->type;
    if (type == Token::NUMBER)
        return track(new Int(std::atoi(body.c_str())));
    if (type == Token::BOOL)
        return track(new Bool(body == "True"));
    return nullptr;
}

Object *Interpreter::evaluate(SetVariable *expression) {
    auto value = expression->value->evaluate(this);
    setVariable(expression->name, value);
    return value;
}

Object *Interpreter::evaluate(Variable *expression) {
//    leaks here?
    return getVariable(expression->body);
}

//TODO: closures don't work for now
//TODO: needs refactoring
Object *Interpreter::evaluate(FunctionExpression *expression) {
    Object *obj = expression->target->evaluate(this);
    if (expression->argsList.size() != obj->functionArguments.size())
        throw "Number of arguments doesn't match";

    addScope();
    for (int i = 0; i < obj->functionArguments.size(); i++) {
        auto arg = expression->argsList[i]->evaluate(this);
        setVariable(obj->functionArguments[i], arg);
    }
    Object *returnObject = nullptr;
    try {
        if (obj->functionBody)
            obj->functionBody->evaluate(this);
        else
            returnObject = obj->__call__(scopes.back());
    } catch (Object *object) {
        returnObject = object;
    } catch (Token::tokenType type) {
//        TODO: move to syntactic errors
//        TODO: now the function still return none on this exception
        std::cout << "Control flow outside loop";
    } catch (const char *other) {
        deleteScope();
        throw other;
    }
    if (!returnObject)
        returnObject = new None;
    track(returnObject);
    deleteScope();
    return returnObject;
}

void Interpreter::evaluate(ReturnStatement *statement) {
    if (statement->expression)
        throw track(statement->expression->evaluate(this));
    throw track(new None);
}

void Interpreter::evaluate(ExpressionStatement *statement) {
    statement->expression->evaluate(this);
}

void Interpreter::evaluate(IfStatement *statement) {
    auto cond = statement->condition->evaluate(this);
    if (cond->asBool()) {
        if (statement->left)
            statement->left->evaluate(this);
    } else if (statement->right)
        statement->right->evaluate(this);
}

void Interpreter::evaluate(WhileStatement *statement) {
    auto cond = statement->condition->evaluate(this);
    while (cond->asBool()) {
        try {
            if (statement->body)
                statement->body->evaluate(this);
        } catch (Token::tokenType type) {
            if (type == Token::BREAK)
                break;
        }
        cond = statement->condition->evaluate(this);
    }
}

void Interpreter::evaluate(ControlFlow *statement) {
    throw statement->type;
}

void Interpreter::evaluate(Block *block) {
    evaluateStatements(block->statements);
}

void Interpreter::evaluate(FunctionDefinition *statement) {
    setVariable(statement->name, new Function(statement->arguments, statement->body));
}