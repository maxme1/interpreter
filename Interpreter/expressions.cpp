#include <iostream>
#include "Interpreter.h"
#include "../Parser/Parser.h"
#include "../Object/Types/Int.h"
#include "../Object/Native.h"
#include "../Object/Types/None.h"
#include "../Object/Class.h"

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
    Object *left = expression->left->evaluate(this), *right = expression->right->evaluate(this);
//    user-defined methods
    auto name = binary.find(expression->token.type);
    if (name != binary.end()) {
        auto method = left->findAttribute(name->second);
        if (method)
            return callOperator(method, {right});
    }

    if (expression->ofType(Token::ADD))
        return track(left->add(right));
    if (expression->ofType(Token::SUB))
        return track(left->subtract(right));
    if (expression->ofType(Token::MUL))
        return track(left->multiply(right));
    if (expression->ofType(Token::DIV))
        return track(left->divide(right));
//    comparison
    if (expression->ofType(Token::EQUAL))
        return track(left->equal(right));
    if (expression->ofType(Token::NOT_EQUAL))
        return track(left->not_equal(right));
    if (expression->ofType(Token::GREATER_OR_EQUAL))
        return track(left->greater_or_equal(right));
    if (expression->ofType(Token::GREATER))
        return track(left->greater(right));
    if (expression->ofType(Token::LESS))
        return track(left->less(right));
    if (expression->ofType(Token::LESS_OR_EQUAL))
        return track(left->less_or_equal(right));
    return nullptr;
}

Object *Interpreter::evaluate(Unary *expression) {
    Object *argument = expression->argument->evaluate(this);
    //    user-defined methods
    auto name = unary.find(expression->token.type);
    if (name != unary.end()) {
        auto method = argument->findAttribute(name->second);
        if (method)
            return callOperator(method, {});
    }

    if (expression->ofType(Token::ADD))
        return track(argument->unary_add());
    if (expression->ofType(Token::SUB))
        return track(argument->unary_subtract());
    if (expression->ofType(Token::BRACKET))
        return argument;
    return nullptr;
}

Object *Interpreter::evaluate(Literal *expression) {
    if (expression->ofType(Token::NUMBER))
// TODO: does'nt look good
        return track(new Int(std::atoi(expression->str().c_str())));
    if (expression->ofType(Token::BOOL))
        return track(new Bool(expression->str() == "True"));
    if (expression->ofType(Token::NONE))
        return track(new None());
    return nullptr;
}

Object *Interpreter::evaluate(SetVariable *expression) {
    auto value = expression->value->evaluate(this);
    setVariable(expression->name, value);
    return value;
}

Object *Interpreter::evaluate(SetAttribute *expression) {
//    TODO: also looks bad
    auto value = expression->value->evaluate(this), target = expression->target->target->evaluate(this);
    target->setAttribute(expression->target->name, value);
    return value;
}

Object *Interpreter::evaluate(Variable *expression) {
//    leaks here?
    return getVariable(expression->name);
}

Object *Interpreter::evaluate(FunctionExpression *expression) {
    Object *object = expression->target->evaluate(this);

    auto classObject = dynamic_cast<Class *>(object);
    if (classObject) {
        auto instance = classObject->__call__({}, this);
        auto init = instance->findAttribute("init");
        if (init)
            callFunction(init, expression->argsList);
        return instance;
    }

    return callFunction(object, expression->argsList);
}

Object *Interpreter::evaluate(GetAttribute *expression) {
    auto target = expression->target->evaluate(this);
    return target->getAttribute(expression->name);
}