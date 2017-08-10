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

ObjPtr Interpreter::evaluate(Binary *expression) {
    ObjPtr left = expression->left->evaluate(this), right = expression->right->evaluate(this);
//    user-defined methods
    auto name = binary.find(expression->token.type);
    if (name != binary.end()) {
        auto method = left->findAttribute(name->second);
        if (method)
            return callOperator(method, {right});
    }

    if (expression->ofType(Token::EQUAL))
        return New(Bool(left == right));

    throw Wrap(new Exception("Operator not defined"));
}

ObjPtr Interpreter::evaluate(Unary *expression) {
    ObjPtr argument = expression->argument->evaluate(this);
    //    user-defined methods
    auto name = unary.find(expression->token.type);
    if (name != unary.end()) {
        auto method = argument->findAttribute(name->second);
        if (method)
            return callOperator(method, {});
    }
    throw Wrap(new Exception("Operator not defined"));
}

ObjPtr Interpreter::evaluate(Literal *expression) {
    if (expression->ofType(Token::NUMBER))
        return New(Int(std::stoi(expression->token.body)));
    if (expression->ofType(Token::BOOL))
        return New(Bool(expression->token.body == "True"));
    if (expression->ofType(Token::NONE))
        return New(None());
    if (expression->ofType(Token::STRING)) {
        auto body = expression->token.body;
        return New(String(body.substr(1, body.size() - 2)));
    }
    return nullptr;
}

ObjPtr Interpreter::evaluate(SetVariable *expression) {
    auto value = expression->value->evaluate(this);
    setVariable(expression->name, value);
    return value;
}

ObjPtr Interpreter::evaluate(SetAttribute *expression) {
//    TODO: also looks bad
    auto value = expression->value->evaluate(this), target = expression->target->target->evaluate(this);
    target->setAttribute(expression->target->name, value);
    return value;
}

ObjPtr Interpreter::evaluate(SetItem *expression) {
//    TODO: also looks bad
    auto target = expression->target->target->evaluate(this);
    auto argument = expression->target->argument->evaluate(this);
    auto value = expression->value->evaluate(this);
    //    user-defined method
    auto method = target->getAttribute("setitem");
    return callOperator(method, {argument, value});
}

ObjPtr Interpreter::evaluate(Variable *expression) {
    return getVariable(expression->name);
}

ObjPtr Interpreter::evaluate(FunctionExpression *expression) {
    ObjPtr object = expression->target->evaluate(this);

    auto classObject = std::dynamic_pointer_cast<Class, Object>(object);
    if (classObject) {
        auto instance = classObject->makeInstance(nullptr);
        auto init = instance->findAttribute("init");
        if (init)
            callFunction(init, expression->argsList);
        else if (!expression->argsList.empty())
            throw Wrap(new Exception("Default constructor does not receive arguments"));
        return instance;
    }

    return callFunction(object, expression->argsList);
}

ObjPtr Interpreter::evaluate(GetItem *expression) {
    auto target = expression->target->evaluate(this);
    auto argument = expression->argument->evaluate(this);
    //    user-defined method
    auto method = target->getAttribute("getitem");
    return callOperator(method, {argument});
}

ObjPtr Interpreter::evaluate(GetAttribute *expression) {
    auto target = expression->target->evaluate(this);
    return target->getAttribute(expression->name);
}