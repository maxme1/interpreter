#include <iostream>
#include "Interpreter.h"
#include "../Parser/Parser.h"
#include "../Object/Types/Int.h"

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

    throw Exception("Operator not defined");
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
    throw Exception("Operator not defined");
}

Object *Interpreter::evaluate(Literal *expression) {
    if (expression->ofType(Token::NUMBER))
// TODO: doesn't look good
        return track(new Int(std::stoi(expression->str())));
    if (expression->ofType(Token::BOOL))
        return track(new Bool(expression->str() == "True"));
    if (expression->ofType(Token::NONE))
        return track(new None());
    if (expression->ofType(Token::STRING)) {
        auto body = expression->str();
        return track(new String(body.substr(1, body.size() - 2)));
    }
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

Object *Interpreter::evaluate(SetItem *expression) {
//    TODO: also looks bad
    auto target = expression->target->target->evaluate(this);
    auto argument = expression->target->argument->evaluate(this);
    auto value = expression->value->evaluate(this);
    //    user-defined method
    auto method = target->getAttribute("setitem");
    return callOperator(method, {argument, value});
}

Object *Interpreter::evaluate(Variable *expression) {
//    leaks here?
    return getVariable(expression->name);
}

Object *Interpreter::evaluate(FunctionExpression *expression) {
    Object *object = expression->target->evaluate(this);

    auto classObject = dynamic_cast<Class *>(object);
    if (classObject) {
        auto instance = classObject->__call__({}, api);
        auto init = instance->findAttribute("init");
        if (init)
            callFunction(init, expression->argsList);
        else if (!expression->argsList.empty())
            throw Exception("Default constructor does not receive arguments");
        return instance;
    }

    return callFunction(object, expression->argsList);
}

Object *Interpreter::evaluate(GetItem *expression) {
    auto target = expression->target->evaluate(this);
    auto argument = expression->argument->evaluate(this);
    //    user-defined method
    auto method = target->getAttribute("getitem");
    return callOperator(method, {argument});
}

Object *Interpreter::evaluate(GetAttribute *expression) {
    auto target = expression->target->evaluate(this);
    return target->getAttribute(expression->name);
}