#include <iostream>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/Int.h"

void Interpreter::interpret(std::string text) {
    Tokenizer t = Tokenizer(text);
    auto tokens = t.tokenize();
    if (t.error) {
        //    TODO: it would be better to know the position in the text on error
        std::cout << "Undefined token";
        return;
    }

    Parser p = Parser(tokens);
    auto statements = p.build();
    if (p.error) {
        return;
    }
    std::cout << statements[0]->str();
    try {
        evaluateStatements(statements);
    } catch (const char *exception) {
        std::cout << exception;
        collect();
    }
}

void Interpreter::collect() {
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

void Interpreter::evaluateStatements(std::vector<Statement *> &statements) {
    for (auto statement : statements) {
        statement->evaluate(this);
        collect();
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

Object *Interpreter::evaluate(Literal *expression) {
    auto body = expression->body;
    auto type = expression->type;
    if (type == Token::NUMBER)
        return new Int(std::atoi(body.c_str()));
    if (type == Token::BOOL)
        return new Bool(body == "True");
    return nullptr;
}

Object *Interpreter::evaluate(SetVariable *expression) {
    auto value = expression->value->evaluate(this);
    garbage.push(value);
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

void Interpreter::evaluate(IfStatement *statement) {
    auto cond = statement->condition->evaluate(this);
    garbage.push(cond);
    if (cond->asBool()) {
        if (statement->left)
            statement->left->evaluate(this);
    } else if (statement->right)
        statement->right->evaluate(this);
}

void Interpreter::evaluate(Block *block) {
    evaluateStatements(block->statements);
}



