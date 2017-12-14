#ifndef walker_EXPRESSION_H
#define walker_EXPRESSION_H

#include <string>
#include <iostream>
#include <utility>
#include "../../Object/Object.h"
#include "../../Tokenizer/Token.h"
#include "../../TreeWalk/Interpreter/Interpreter.h"
#include "../Statement/Statement.h"

struct Object;
struct Expression {
    Token token;
public:
public:
    virtual ObjPtr visit(TreeWalker *walker) = 0;

    explicit Expression(Token token) : token(std::move(token)) {};
    virtual ~Expression() = default;

    bool ofType(Token::tokenType type) { return type == token.type; }

    virtual std::string str() { return token.body; };
};

struct Binary : public Expression {
    Expression *left, *right;
public:

    ObjPtr visit(TreeWalker *walker) override {
        return walker->visit(this);
    }

    Binary(Token token, Expression *left, Expression *right);
    ~Binary() override;
    std::string str() override;
};

struct Unary : public Expression {
    friend struct walker;

public:
    ObjPtr visit(TreeWalker *walker) override {
        return walker->visit(this);
    }

    Expression *argument;
public:
    Unary(Token token, Expression *argument);
    ~Unary() override;
    std::string str() override;
};

struct Literal : public Expression {
    friend struct walker;
private:
public:
    ObjPtr visit(TreeWalker *walker) override {
        return walker->visit(this);
    }

public:
    explicit Literal(const Token &token) : Expression(token) {}
};

struct Variable : public Expression {
    long level{-1};
public:
    ObjPtr visit(TreeWalker *walker) override {
        return walker->visit(this);
    }

public:
    explicit Variable(const Token &token) : Expression(token), name(token.body) {}

    std::string name;
};

struct CallExpression : public Expression {
    friend struct walker;

public:
    ObjPtr visit(TreeWalker *walker) override {
        return walker->visit(this);
    }

    Expression *target;
    std::vector<Expression *> argsList;
public:
    CallExpression(Token token, Expression *target, std::vector<Expression *> argsList);
    ~CallExpression() override;
    std::string str() override;
};

struct GetItem : public Expression {
    friend struct walker;
    friend struct SetItem;

public:
    ObjPtr visit(TreeWalker *walker) override {
        return walker->visit(this);
    }

    Expression *target;
    Expression *argument;
public:
    GetItem(Token token, Expression *target, Expression *argument);
    ~GetItem() override;
    std::string str() override;
};

struct GetAttribute : public Expression {
    friend struct walker;
    friend struct SetAttribute;

public:
    ObjPtr visit(TreeWalker *walker) override {
        return walker->visit(this);
    }

    Expression *target;
    std::string name;
public:
    GetAttribute(Token token, Expression *target, std::string name);
    ~GetAttribute() override;
    std::string str() override;
};

struct SetVariable : public Expression {
    friend struct walker;
    long level = -1;

public:
    ObjPtr visit(TreeWalker *walker) override {
        return walker->visit(this);
    }

    std::string name;
    Expression *value;

public:
    SetVariable(Token token, std::string name, Expression *value);
    ~SetVariable() override;
    std::string str() override;
};

struct SetAttribute : public Expression {
    friend struct walker;

public:
    ObjPtr visit(TreeWalker *walker) override {
        return walker->visit(this);
    }

    Expression *value;
    GetAttribute *target;

public:
    SetAttribute(const Token &token, GetAttribute *target, Expression *value);
    ~SetAttribute() override;
    std::string str() override;
};

struct SetItem : public Expression {
    friend struct walker;

public:
    ObjPtr visit(TreeWalker *walker) override {
        return walker->visit(this);
    }

    Expression *value;
    GetItem *target;

public:
    SetItem(const Token &token, GetItem *target, Expression *value);
    ~SetItem() override;
    std::string str() override;
};

#endif //walker_EXPRESSION_H
