#include <iostream>
#include "Interpreter.h"
#include "../Parser/Parser.h"
#include "../Object/Types/Int.h"
#include "../Object/Types/None.h"

std::map<Token::tokenType, std::string> binary = {
        {Token::ADD,              "add"},
        {Token::SUB,              "sub"},
        {Token::MUL,              "mul"},
        {Token::DIV,              "div"},
        {Token::EQUAL,            "eq"},
        {Token::GREATER,          "gr"},
        {Token::GREATER_OR_EQUAL, "geq"},
        {Token::LESS,             "ls"},
        {Token::LESS_OR_EQUAL,    "leq"},
        {Token::NOT_EQUAL,        "neq"}
};

std::map<Token::tokenType, std::string> unary = {
        {Token::ADD, "uadd"},
        {Token::SUB, "usub"},
};

Object *Interpreter::evaluate(Binary *expression) {
    Object *left = track(expression->left->evaluate(this)), *right = track(expression->right->evaluate(this));
//    user-defined methods
    auto name = binary.find(expression->token.type);
    if (name != binary.end()) {
        auto method = track(left->findAttribute(name->second));
        if (method)
            return callOperator(method, {right});
    }

    if (expression->ofType(Token::EQUAL))
        return track(new Bool(left == right));

    throw Wrap(new Exception("Operator not defined"));
}

Object *Interpreter::evaluate(Unary *expression) {
    Object *argument = track(expression->argument->evaluate(this));
    //    user-defined methods
    auto name = unary.find(expression->token.type);
    if (name != unary.end()) {
        auto method = track(argument->findAttribute(name->second));
        if (method)
            return callOperator(method, {});
    }
    throw Wrap(new Exception("Operator not defined"));
}

Object *Interpreter::evaluate(Literal *expression) {
    if (expression->ofType(Token::NUMBER))
        return track(new Int(std::stoi(expression->token.body)));
    if (expression->ofType(Token::BOOL))
        return track(new Bool(expression->token.body == "True"));
    if (expression->ofType(Token::NONE))
        return track(new None());
    if (expression->ofType(Token::STRING)) {
        auto body = expression->token.body;
        return track(new String(body.substr(1, body.size() - 2)));
    }
    return nullptr;
}

Object *Interpreter::evaluate(SetVariable *expression) {
    auto value = track(expression->value->evaluate(this));
    setVariable(expression->name, value);
    return value;
}

Object *Interpreter::evaluate(SetAttribute *expression) {
//    TODO: also looks bad
    auto value = track(expression->value->evaluate(this)), target = track(expression->target->target->evaluate(this));
    target->setAttribute(expression->target->name, value);
    return value;
}

Object *Interpreter::evaluate(SetItem *expression) {
//    TODO: also looks bad
    auto target = track(expression->target->target->evaluate(this));
    auto argument = track(expression->target->argument->evaluate(this));
    auto value = track(expression->value->evaluate(this));
    //    user-defined method
    auto method = target->getAttribute("setitem");
    return callOperator(method, {argument, value});
}

Object *Interpreter::evaluate(Variable *expression) {
    return getVariable(expression->name);
}

Object *Interpreter::evaluate(FunctionExpression *expression) {
    Object *object = track(expression->target->evaluate(this));

    auto classObject = dynamic_cast<Class *>(object);
    if (classObject) {
        auto instance = track(classObject->makeInstance(nullptr));
        auto init = track(instance->findAttribute("init"));
        if (init)
            track(callFunction(init, expression->argsList));
        else if (!expression->argsList.empty())
            throw Wrap(new Exception("Default constructor does not receive arguments"));
        return instance;
    }

    return callFunction(object, expression->argsList);
}

Object *Interpreter::evaluate(GetItem *expression) {
    auto target = track(expression->target->evaluate(this));
    auto argument = track(expression->argument->evaluate(this));
    //    user-defined method
    auto method = target->getAttribute("getitem");
    return callOperator(method, {argument});
}

Object *Interpreter::evaluate(GetAttribute *expression) {
    auto target = track(expression->target->evaluate(this));
    return target->getAttribute(expression->name);
}