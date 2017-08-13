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

    bool ofType(Token::tokenType type) { return type == token.type; }

    virtual std::string str() { return token.body; };
};

typedef std::shared_ptr<Expression> ExprPtr;

class Binary : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    ExprPtr left, right;
public:
    Binary(Token token, ExprPtr left, ExprPtr right);

    std::string str() override;
};

class Unary : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    ExprPtr argument;
public:
    Unary(Token token, ExprPtr argument);

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

    ExprPtr target;
    std::vector<ExprPtr> argsList;
public:
    FunctionExpression(Token token, ExprPtr target, std::vector<ExprPtr> argsList);

    std::string str() override;
};

class GetItem : public Expression {
    friend class Interpreter;

    friend class SetItem;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    ExprPtr target;
    ExprPtr argument;
public:
    GetItem(Token token, ExprPtr target, ExprPtr argument);

    std::string str() override;
};

class GetAttribute : public Expression {
    friend class Interpreter;

    friend class SetAttribute;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    ExprPtr target;
    std::string name;
public:
    GetAttribute(Token token, ExprPtr target, std::string name);

    std::string str() override;
};

class SetVariable : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    std::string name;
    ExprPtr value;

public:
    SetVariable(Token token, std::string name, ExprPtr value);

    std::string str() override;
};

class SetAttribute : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    ExprPtr value;
    std::shared_ptr<GetAttribute> target;

public:
    SetAttribute(const Token &token, std::shared_ptr<GetAttribute> target, ExprPtr value);

    std::string str() override;
};

class SetItem : public Expression {
    friend class Interpreter;

    ObjPtr evaluate(Interpreter *interpreter) override {
        return interpreter->evaluate(this);
    }

    ExprPtr value;
    std::shared_ptr<GetItem> target;

public:
    SetItem(const Token &token, std::shared_ptr<GetItem> target, ExprPtr value);

    std::string str() override;
};

#endif //INTERPRETER_EXPRESSION_H
