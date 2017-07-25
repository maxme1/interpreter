#include <iostream>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/Int.h"

void Interpreter::interpret(std::string text) {
    Tokenizer t = Tokenizer(text);
    auto tokens = t.tokenize();
    if (t.error)
        std::cout << "ERROR!";

    Parser p = Parser(tokens);
    auto statements = p.build();
    std::cout << statements[0]->str();
    evaluateStatements(statements);
    statements.clear();
}

void Interpreter::evaluateStatements(std::vector<Statement *> &statements) {
    for (auto statement : statements) {
        statement->evaluate(this);

        if (!garbage.empty()) {
            std::cout << "garbage: ";
            while (!garbage.empty()) {
                auto obj = garbage.top();
                garbage.pop();
                if (obj and obj->zombie()) {
                    std::cout << obj->str() << " ";
                    delete obj;
                }
            }
            std::cout << "\n";
        }
    }
}

Object *Interpreter::evaluate(Binary *expression) {
    Object *left = expression->left->evaluate(this), *right = expression->right->evaluate(this);
    garbage.push(left);
    garbage.push(right);
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
    garbage.push(argument);
    if (expression->type == Token::ADD)
        return argument->uadd();
    if (expression->type == Token::SUB)
        return argument->usub();
    if (expression->type == Token::BRACKET)
        return argument;
    return nullptr;
}

Object *Interpreter::evaluate(Number *expression) {
    return new Int(expression->value);
}

Object *Interpreter::evaluate(SetVariable *expression) {
    auto value = expression->value->evaluate(this);
    globalScope.setAttribute(expression->name, value);
    return value;
}

Object *Interpreter::evaluate(Variable *expression) {
    return globalScope.getAttribute(expression->body);
}

void Interpreter::evaluate(ExpressionStatement *statement) {
//    TODO: probably can collect all here
    auto obj = statement->expression->evaluate(this);
    garbage.push(obj);
    std::cout << obj->str() << "\n";
}

void Interpreter::evaluate(Block *block) {
    evaluateStatements(block->statements);
}

