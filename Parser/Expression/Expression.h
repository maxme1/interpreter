#ifndef INTERPRETER_EXPRESSION_H
#define INTERPRETER_EXPRESSION_H

#include <string>
#include <iostream>
#include <utility>
#include "../../Object/Object.h"
#include "../../Tokenizer/Token.h"
#include "../../Interpreter/Interpreter.h"
#include "../Statement/Statement.h"


class Object;
class Expression {
    friend class Interpreter;
    virtual ObjPtr evaluate(Interpreter *interpreter) = 0;

    Token token;
public:
    explicit Expression(Token token) : token(std::move(token)) {};
    virtual ~Expression() = default;

    bool ofType(Token::tokenType type) { return type == token.type; }

    virtual std::string str() { return token.body; };
};

class Binary : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    Expression *left, *right;
public:
    Binary(Token token, Expression *left, Expression *right);
    ~Binary() override;
    std::string str() override;
};

class Unary : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    Expression *argument;
public:
    Unary(Token token, Expression *argument);
    ~Unary() override;
    std::string str() override;
};

class Literal : public Expression {
    friend class Interpreter;
private:
    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

public:
    explicit Literal(const Token &token) : Expression(token) {}
};

class Variable : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

public:
    explicit Variable(const Token &token) : Expression(token), name(token.body) {}

    std::string name;
};

class FunctionExpression : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    Expression *target;
    std::vector<Expression *> argsList;
public:
    FunctionExpression(Token token, Expression *target, std::vector<Expression *> argsList);
    ~FunctionExpression() override;
    std::string str() override;
};

class GetItem : public Expression {
    friend class Interpreter;
    friend class SetItem;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    Expression *target;
    Expression *argument;
public:
    GetItem(Token token, Expression *target, Expression *argument);
    ~GetItem() override;
    std::string str() override;
};

class GetAttribute : public Expression {
    friend class Interpreter;
    friend class SetAttribute;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    Expression *target;
    std::string name;
public:
    GetAttribute(Token token, Expression *target, std::string name);
    ~GetAttribute() override;
    std::string str() override;
};

class SetVariable : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    std::string name;
    Expression *value;

public:
    SetVariable(Token token, std::string name, Expression *value);
    ~SetVariable() override;
    std::string str() override;
};

class SetAttribute : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    Expression *value;
    GetAttribute *target;

public:
    SetAttribute(const Token &token, GetAttribute *target, Expression *value);
    ~SetAttribute() override;
    std::string str() override;
};

class SetItem : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    Expression *value;
    GetItem *target;

public:
    SetItem(const Token &token, GetItem *target, Expression *value);
    ~SetItem() override;
    std::string str() override;
};

#endif //INTERPRETER_EXPRESSION_H
