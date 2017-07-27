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
    virtual Object *evaluate(Interpreter *interpreter) = 0;

    std::string body;
protected:
    Token::tokenType type;
public:
    Expression() = default;
    virtual ~Expression() = default;;

    Expression(const std::string &body, Token::tokenType type) : body(body), type(type) {};

    bool ofType(Token::tokenType _type) { return _type == type; }

    virtual std::string str() { return body; };
};

class Binary : public Expression {
    friend class Interpreter;

    Object *evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    Expression *left, *right;

public:
    Binary(const std::string &body, Token::tokenType type, Expression *left, Expression *right);
    ~Binary();

    std::string str() override;
};

class Unary : public Expression {
    friend class Interpreter;

    Object *evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    Expression *argument;
public:

    Unary(const std::string &body, Token::tokenType type, Expression *argument);
    virtual ~Unary();
    std::string str() override;
};

class Literal : public Expression {
    friend class Interpreter;

    Object *evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

public:
    Literal(const std::string &body, Token::tokenType type) : Expression(body, type) {}
};

class Variable : public Expression {
    friend class Interpreter;

    Object *evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

public:
    Variable(const std::string &body, Token::tokenType type) : Expression(body, type) {}
};

class FunctionExpression : public Expression {
    friend class Interpreter;

    Object *evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    Expression *target, *argument;
public:
    FunctionExpression(Expression *target, Expression *argument);
    std::string str() override;
};

class SetVariable : public Expression {
    friend class Interpreter;

    Object *evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    std::string name;
    Expression *value;

public:
    SetVariable(const std::string &name, Expression *value);
    ~SetVariable();
    std::string str() override;
};

#endif //INTERPRETER_EXPRESSION_H
