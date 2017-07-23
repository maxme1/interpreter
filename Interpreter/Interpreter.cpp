#include <iostream>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"

void Interpreter::interpret(std::string text) {
    Tokenizer t = Tokenizer(text);
    auto tokens = t.tokenize();
    if (t.error)
        std::cout << "ERROR!";

    Parser p = Parser(tokens);
    auto statements = p.build();
    for (auto &&statement : statements) {
        std::cout << statement->str() << "\n";
        statement->evaluate(this);
    }
}

Object *Interpreter::evaluate(Binary *expression) {
    Object *left = expression->left->evaluate(this), *right = expression->right->evaluate(this);
    if (expression->type == Token::ADD)
        return left->add(right);
    if (expression->type == Token::SUB)
        return left->sub(right);
    if (expression->type == Token::MUL)
        return left->mul(right);
    if (expression->type == Token::DIV)
        return left->div(right);
    return nullptr;
}

Object *Interpreter::evaluate(Unary *expression) {
    Object *argument = expression->argument->evaluate(this);
    if (expression->type == Token::ADD)
        return argument->uadd();
    if (expression->type == Token::SUB)
        return argument->usub();
    if (expression->type == Token::BRACKET)
        return argument;
    return nullptr;
}

Object *Interpreter::evaluate(NumberToken *expression) {
    return new Number(expression->value);
}

Object *Interpreter::evaluate(SetVariable *expression) {
    auto value = expression->value->evaluate(this);
    globalScope.setAttribute(expression->name, value);
    return value;
}

void Interpreter::evaluate(ExpressionStatement *expression) {
    auto expr = expression->expression->evaluate(this);
    std::cout << expr->str() << "\n";
}

Object *Interpreter::evaluate(Variable *expression) {
    return globalScope.getAttribute(expression->body);
}

