#ifndef INTERPRETER_STATEMENT_H
#define INTERPRETER_STATEMENT_H


#include <string>
#include "../../Interpreter/Interpreter.h"

class Expression;

class Statement {
public:
    virtual ~Statement() {};

    virtual std::string str() = 0;
    virtual void evaluate(Interpreter *interpreter) = 0;
};

class Block : public Statement {
    friend class Interpreter;

    std::vector<Statement *> statements;
public:
    Block(const std::vector<Statement *> &statements);
    ~Block();
    std::string str() override;
    void evaluate(Interpreter *interpreter) override;
};

class ExpressionStatement : public Statement {
    friend class Interpreter;

    Expression *expression;

public:
    ExpressionStatement(Expression *expression);
    ~ExpressionStatement();
    void evaluate(Interpreter *interpreter) override;
    std::string str() override;
};

class IfStatement : public Statement {
    friend class Interpreter;

    Expression *condition;
    Statement *left, *right;

public:
    IfStatement(Expression *condition, Statement *left = nullptr, Statement *right = nullptr);
    ~IfStatement();
    void evaluate(Interpreter *interpreter) override;
    std::string str() override;
};

class WhileStatement : public Statement {
    friend class Interpreter;

    Expression *condition;
    Statement *body;

public:
    WhileStatement(Expression *condition, Statement *body = nullptr);
    ~WhileStatement();
    void evaluate(Interpreter *interpreter) override;
    std::string str() override;
};


#endif //INTERPRETER_STATEMENT_H
