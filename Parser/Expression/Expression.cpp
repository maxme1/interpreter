#include <iostream>
#include "Expression.h"

Expression::Expression(const std::string &body) : body(body) {}

Expression::Expression(const std::string &body, Token::tokenType type) : body(body), type(type) {}

Binary::Binary(const std::string &body, Token::tokenType type,
               Expression *left, Expression *right) : Expression(body, type), left(left), right(right) {}

std::string Binary::str() {
    return "(" + left->str() + " " + Expression::str() + " " + right->str() + ")";
}

Object *Binary::evaluate(Interpreter *interpreter) {
    return interpreter->evaluate(this);
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

Object *Unary::evaluate(Interpreter *interpreter) {
    return interpreter->evaluate(this);
}

Unary::~Unary() {
    delete argument;
}

Number::Number(const std::string &body, int value) : Expression(body), value(value) {}

std::string Number::str() {
    return std::to_string(value);
}

Object *Number::evaluate(Interpreter *interpreter) {
    return interpreter->evaluate(this);
}

Variable::Variable(const std::string &body, Token::tokenType type) : Expression(body, type) {}

Object *Variable::evaluate(Interpreter *interpreter) {
    return interpreter->evaluate(this);
}

SetVariable::SetVariable(const std::string &name, Expression *value) : name(name), value(value) {}

std::string SetVariable::str() {
    return "(" + name + " = " + value->str() + ")";
}

Object *SetVariable::evaluate(Interpreter *interpreter) {
    return interpreter->evaluate(this);
}

SetVariable::~SetVariable() {
    delete value;
}
