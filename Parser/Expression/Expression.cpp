#include <iostream>
#include "Expression.h"

Binary::Binary(const std::string &body, Token::tokenType type,
               Expression *left, Expression *right) : Expression(body, type), left(left), right(right) {}

std::string Binary::str() {
    return "(" + left->str() + " " + Expression::str() + " " + right->str() + ")";
}

Binary::~Binary() {
    delete left, right;
}

Unary::Unary(const std::string &body, Token::tokenType type,
             Expression *argument) : Expression(body, type), argument(argument) {}

std::string Unary::str() {
    if (type == Token::BRACKET)
        return argument->str();
    return Expression::str() + argument->str();
}

Unary::~Unary() {
    delete argument;
}

FunctionExpression::FunctionExpression(Expression *target, Expression *argument) : target(target), argument(argument) {}

std::string FunctionExpression::str() {
    return target->str() + "(" + argument->str() + ")";
}

SetVariable::SetVariable(const std::string &name, Expression *value) : name(name), value(value) {}

std::string SetVariable::str() {
    return "(" + name + " = " + value->str() + ")";
}

SetVariable::~SetVariable() {
    delete value;
}
