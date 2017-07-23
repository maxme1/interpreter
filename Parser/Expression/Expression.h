#ifndef INTERPRETER_EXPRESSION_H
#define INTERPRETER_EXPRESSION_H

#include <string>
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

    bool ofType(Token::tokenType _type) { return _type == type; }

    virtual std::string str() { return body; };
    virtual Object *evaluate(Interpreter *interpreter) = 0;
};

class Binary : public Expression {
    friend class Interpreter;

    Expression *left, *right;

public:
    Binary(const std::string &body, Token::tokenType type, Expression *left, Expression *right);

    std::string str() override;
    Object *evaluate(Interpreter *interpreter);
};

class Unary : public Expression {
    friend class Interpreter;

    Expression *argument;

public:
    Unary(const std::string &body, Token::tokenType type, Expression *argument);

    std::string str() override;
    Object *evaluate(Interpreter *interpreter);
};

class NumberToken : public Expression {
    friend class Interpreter;

    int value;
public:
    NumberToken(const std::string &body, int value);

    std::string str() override;

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
    Object *evaluate(Interpreter *interpreter) override;
    std::string str() override;
    SetVariable(const std::string &name, Expression *value);
};

#endif //INTERPRETER_EXPRESSION_H
