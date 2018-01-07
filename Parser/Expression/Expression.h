#ifndef walker_EXPRESSION_H
#define walker_EXPRESSION_H

#include <string>
#include <iostream>
#include <utility>
#include "../../Object/Object.h"
#include "../../Tokenizer/Token.h"
#include "../../TreeWalk/Interpreter/Interpreter.h"
#include "../Statement/Statement.h"
#include "../../ExceptionWrapper.h"

struct Object;
struct Expression {
    Token token;
public:
    virtual ObjPtr visit(TreeWalker *walker) = 0;

    explicit Expression(Token token) : token(std::move(token)) {};

    bool ofType(Token::tokenType type) { return type == token.type; }

    virtual std::string str() { return token.body; };
    typedef shared(Expression) ptr;
};

struct Binary : public Expression {
    shared(Expression) left, right;
public:

    ObjPtr visit(TreeWalker *walker) override {
        try {
            return walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

    Binary(Token token, shared(Expression) left, shared(Expression) right);
    std::string str() override;
};

struct Unary : public Expression {
    friend struct walker;

public:
    ObjPtr visit(TreeWalker *walker) override {
        try {
            return walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

    shared(Expression) argument;
public:
    Unary(Token token, shared(Expression) argument);
    std::string str() override;
};

struct Literal : public Expression {
    friend struct walker;
private:
public:
    ObjPtr visit(TreeWalker *walker) override {
        try {
            return walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit Literal(const Token &token) : Expression(token) {}
};

struct Variable : public Expression {
    long level{-1};
public:
    ObjPtr visit(TreeWalker *walker) override {
        try {
            return walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit Variable(const Token &token) : Expression(token), name(token.body) {}

    std::string name;
};

struct CallExpression : public Expression {
    friend struct walker;

public:
    ObjPtr visit(TreeWalker *walker) override {
        try {
            return walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

    shared(Expression) target;
    std::vector<shared(Expression) > argsList;
    std::vector<shared(SetVariable) > kwargs;
public:
    CallExpression(Token token, shared(Expression) target, std::vector<shared(Expression) > argsList,
                   std::vector<shared(SetVariable) > kwargs);
    std::string str() override;
};

struct SuperClass : public Expression {
    friend struct walker;

    long level{-1};
public:
    ObjPtr visit(TreeWalker *walker) override {
        try {
            return walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit SuperClass(Token attribute);
    std::string str() override;
};

struct GetItem : public Expression {
    friend struct walker;
    friend struct SetItem;

public:
    ObjPtr visit(TreeWalker *walker) override {
        try {
            return walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

    shared(Expression) target, argument;
public:
    GetItem(Token token, shared(Expression) target, shared(Expression) argument);
    std::string str() override;
};

struct GetAttribute : public Expression {
    friend struct walker;
    friend struct SetAttribute;

public:
    ObjPtr visit(TreeWalker *walker) override {
        try {
            return walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

    shared(Expression) target;
    std::string name;
public:
    GetAttribute(Token token, shared(Expression) target, std::string name);
    std::string str() override;
};

struct SetVariable : public Expression {
    friend struct walker;
    long level = -1;

public:
    ObjPtr visit(TreeWalker *walker) override {
        try {
            return walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

    std::string name;
    shared(Expression) value;

public:
    SetVariable(Token token, std::string name, shared(Expression) value);
    std::string str() override;
};

struct SetAttribute : public Expression {
    friend struct walker;

public:
    ObjPtr visit(TreeWalker *walker) override {
        try {
            return walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

    shared(Expression) value;
    shared(GetAttribute) target;

public:
    SetAttribute(const Token &token, shared(GetAttribute) target, shared(Expression) value);
    std::string str() override;
};

struct SetItem : public Expression {
    friend struct walker;

public:
    ObjPtr visit(TreeWalker *walker) override {
        try {
            return walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

    shared(Expression) value;
    shared(GetItem) target;

public:
    SetItem(const Token &token, shared(GetItem) target, shared(Expression) value);
    std::string str() override;
};

#endif //walker_EXPRESSION_H
