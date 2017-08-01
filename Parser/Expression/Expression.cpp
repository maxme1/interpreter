#include <iostream>
#include <utility>
#include "Expression.h"

Binary::Binary(Token token, Expression *left, Expression *right) :
        Expression(std::move(token)), left(left), right(right) {}

std::string Binary::str() {
    return "(" + left->str() + " " + Expression::str() + " " + right->str() + ")";
}

Binary::~Binary() {
    delete left, right;
}

Unary::Unary(Token token, Expression *argument) : Expression(std::move(token)), argument(argument) {}

std::string Unary::str() {
    if (ofType(Token::BRACKET))
        return argument->str();
    return Expression::str() + argument->str();
}

Unary::~Unary() {
    delete argument;
}

FunctionExpression::FunctionExpression(Token token, Expression *target, std::vector<Expression *> argsList) :
        Expression(std::move(token)), target(target), argsList(std::move(argsList)) {}

std::string FunctionExpression::str() {
    std::string result = target->str() + "(";
    for (auto argument : argsList) {
        result += argument->str();
    }
    return result + ")";
}

FunctionExpression::~FunctionExpression() {
    delete target;
    for (auto &&item : argsList) {
        delete item;
    }
}

GetAttribute::GetAttribute(Token token, Expression *target, std::string name) :
        Expression(std::move(token)), target(target), name(std::move(name)) {}

std::string GetAttribute::str() {
    return target->str() + "." + name;
}

GetAttribute::~GetAttribute() {
    delete target;
}

SetVariable::SetVariable(Token token, std::string name, Expression *value) :
        Expression(std::move(token)), name(std::move(name)), value(value) {}

std::string SetVariable::str() {
    return "(" + name + " = " + value->str() + ")";
}

SetVariable::~SetVariable() {
    delete value;
}

SetAttribute::SetAttribute(const Token &token, GetAttribute *target, Expression *value)
        : Expression(token), target(target), value(value) {}

std::string SetAttribute::str() {
    return "(" + target->str() + "." + target->name + " = " + value->str() + ")";
}

SetAttribute::~SetAttribute() {
    delete target, value;
}