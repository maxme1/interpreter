#ifndef INTERPRETER_EXPRESSION_H
#define INTERPRETER_EXPRESSION_H

#include <string>
#include <iostream>
#include "../../Object/Object.h"
#include "../../Tokenizer/Token.h"
#include "../../Interpreter/Interpreter.h"
#include "../Statement/Statement.h"

class Expression {
    friend class Interpreter;

    std::string body;
protected:
    Token::tokenType type;
public:
    Expression() {};
    Expression(const std::string &body);
    Expression(const std::string &body, Token::tokenType type);

    virtual ~Expression() {};


    bool ofType(Token::tokenType _type) { return _type == type; }

    virtual std::string str() { return body; };
    virtual Object *evaluate(Interpreter *interpreter) = 0;
};

class Binary : public Expression {
    friend class Interpreter;

    Expression *left, *right;

public:
    Binary(const std::string &body, Token::tokenType type, Expression *left, Expression *right);
    ~Binary();

    std::string str() override;
    Object *evaluate(Interpreter *interpreter);
};

class Unary : public Expression {
    friend class Interpreter;

    Expression *argument;

public:
    Unary(const std::string &body, Token::tokenType type, Expression *argument);
    virtual ~Unary();
    std::string str() override;
    Object *evaluate(Interpreter *interpreter);
};

class Literal : public Expression {
    friend class Interpreter;

public:
    Literal(const std::string &body, Token::tokenType type);

    Object *evaluate(Interpreter *interpreter);
};

class Variable : public Expression {
    friend class Interpreter;

public:
    Variable(const std::string &body, Token::tokenType type);
    Object *evaluate(Interpreter *interpreter) override;
};

class SetVariable : public Expression {
    friend class Interpreter;

    std::string name;
    Expression *value;

public:
    SetVariable(const std::string &name, Expression *value);
    ~SetVariable();
    Object *evaluate(Interpreter *interpreter) override;
    std::string str() override;
};

#endif //INTERPRETER_EXPRESSION_H
