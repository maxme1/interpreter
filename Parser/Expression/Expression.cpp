#include <iostream>
#include <utility>
#include "Expression.h"

Binary::Binary(Token token, shared(Expression) left, shared(Expression) right) :
        Expression(std::move(token)), left(left), right(right) {}

std::string Binary::str() {
    return "(" + left->str() + " " + Expression::str() + " " + right->str() + ")";
}

Unary::Unary(Token token, shared(Expression) argument) : Expression(std::move(token)), argument(argument) {}

std::string Unary::str() {
    if (ofType(Token::BRACKET_OPEN))
        return argument->str();
    return Expression::str() + argument->str();
}

CallExpression::CallExpression(Token token, shared(Expression) target, std::vector<shared(Expression) > argsList,
                               std::vector<shared(SetVariable) > kwargs)
        : Expression(std::move(token)), target(target), argsList(std::move(argsList)), kwargs(kwargs) {}

std::string CallExpression::str() {
    std::string result = target->str() + "(";
    for (auto argument : argsList) {
        result += argument->str();
    }
    return result + ")";
}

SuperClass::SuperClass(Token attribute) : Expression(attribute) {}

std::string SuperClass::str() {
    return "super." + token.body;
}

GetItem::GetItem(Token token, shared(Expression) target, shared(Expression) argument) :
        Expression(std::move(token)), target(target), argument(argument) {}

std::string GetItem::str() {
    return target->str() + "[" + argument->str() + "]";
}

GetAttribute::GetAttribute(Token token, shared(Expression) target, std::string name) :
        Expression(std::move(token)), target(target), name(std::move(name)) {}

std::string GetAttribute::str() {
    return target->str() + "." + name;
}

SetVariable::SetVariable(Token token, std::string name, shared(Expression) value) :
        Expression(std::move(token)), name(std::move(name)), value(value) {}

std::string SetVariable::str() {
    return "(" + name + " = " + value->str() + ")";
}

SetAttribute::SetAttribute(const Token &token, shared(GetAttribute) target, shared(Expression) value)
        : Expression(token), target(target), value(value) {}

std::string SetAttribute::str() {
    return "(" + target->str() + "." + target->name + " = " + value->str() + ")";
}

SetItem::SetItem(const Token &token, shared(GetItem) target, shared(Expression) value) :
        Expression(token), target(target), value(value) {}

std::string SetItem::str() {
    return "(" + target->str() + "[" + target->argument->str() + "] = " + value->str() + ")";
}
