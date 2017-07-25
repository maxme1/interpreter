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

class ExpressionStatement : public Statement {
    friend class Interpreter;

    Expression *expression;

public:
    ExpressionStatement(Expression *expression);
    ~ExpressionStatement();
    void evaluate(Interpreter *interpreter) override;
    std::string str() override;
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

#endif //INTERPRETER_STATEMENT_H
