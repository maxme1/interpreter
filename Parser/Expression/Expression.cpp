#include <iostream>
#include <utility>
#include "Expression.h"

Binary::Binary(Token token, ExprPtr left, ExprPtr right) :
        Expression(std::move(token)), left(left), right(right) {}

std::string Binary::str() {
    return "(" + left->str() + " " + Expression::str() + " " + right->str() + ")";
}

Unary::Unary(Token token, ExprPtr argument) : Expression(std::move(token)), argument(argument) {}

std::string Unary::str() {
    if (ofType(Token::BRACKET_OPEN))
        return argument->str();
    return Expression::str() + argument->str();
}

FunctionExpression::FunctionExpression(Token token, ExprPtr target, std::vector<ExprPtr> argsList) :
        Expression(std::move(token)), target(target), argsList(std::move(argsList)) {}

std::string FunctionExpression::str() {
    std::string result = target->str() + "(";
    for (auto argument : argsList) {
        result += argument->str();
    }
    return result + ")";
}

GetItem::GetItem(Token token, ExprPtr target, ExprPtr argument) :
        Expression(std::move(token)), target(target), argument(argument) {}

std::string GetItem::str() {
    return target->str() + "[" + argument->str() + "]";
}

GetAttribute::GetAttribute(Token token, ExprPtr target, std::string name) :
        Expression(std::move(token)), target(target), name(std::move(name)) {}

std::string GetAttribute::str() {
    return target->str() + "." + name;
}

SetVariable::SetVariable(Token token, std::string name, ExprPtr value) :
        Expression(std::move(token)), name(std::move(name)), value(value) {}

std::string SetVariable::str() {
    return "(" + name + " = " + value->str() + ")";
}

SetAttribute::SetAttribute(const Token &token, std::shared_ptr<GetAttribute> target, ExprPtr value)
        : Expression(token), target(target), value(value) {}

std::string SetAttribute::str() {
    return "(" + target->str() + "." + target->name + " = " + value->str() + ")";
}

SetItem::SetItem(const Token &token, std::shared_ptr<GetItem> target, ExprPtr value) :
        Expression(token), target(target), value(value) {}

std::string SetItem::str() {
    return "(" + target->str() + "[" + target->argument->str() + "] = " + value->str() + ")";
}
