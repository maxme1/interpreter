#ifndef INTERPRETER_STATEMENT_H
#define INTERPRETER_STATEMENT_H


#include <string>
#include "../../Interpreter/Interpreter.h"
#include "../../Tokenizer/Token.h"

class Expression;

class Statement {
public:
    virtual void evaluate(Interpreter *interpreter) = 0;
    virtual ~Statement() = default;
    virtual std::string str() = 0;
};

class Block : public Statement {
    friend class Interpreter;

    std::vector<Statement *> statements;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    explicit Block(const std::vector<Statement *> &statements);
    ~Block() override;
    std::string str() override;
};

class ExpressionStatement : public Statement {
    friend class Interpreter;

    Expression *expression;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    explicit ExpressionStatement(Expression *expression);
    ~ExpressionStatement() override;
    std::string str() override;
};

class IfStatement : public Statement {
    friend class Interpreter;

    Expression *condition;
    Statement *left, *right;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    explicit IfStatement(Expression *condition, Statement *left = nullptr, Statement *right = nullptr);
    ~IfStatement() override;
    std::string str() override;
};

class WhileStatement : public Statement {
    friend class Interpreter;

    Expression *condition;
    Statement *body;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    explicit WhileStatement(Expression *condition, Statement *body = nullptr);
    ~WhileStatement() override;
    std::string str() override;
};

class ControlFlow : public Statement {
    friend class Interpreter;
    Token::tokenType type;
    std::string body;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    ControlFlow(Token::tokenType type, const std::string &body);
    std::string str() override;
};

class ReturnStatement : public Statement {
    friend class Interpreter;
    Expression *expression;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    ReturnStatement(Expression *expression = nullptr);
    ~ReturnStatement() override;
    std::string str() override;
};

class FunctionDefinition : public Statement {
    friend class Interpreter;
    Statement *body;
    std::vector<std::string> arguments;
    std::string name;
    bool unlimited;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    FunctionDefinition(const std::string &name, const std::vector<std::string> &arguments, Statement *body,
                       bool unlimited);
    ~FunctionDefinition() override;
    std::string str() override;
};

class ClassDefinition : public Statement {
    friend class Interpreter;
    Statement *body;
    Expression *superclass;
    std::string name;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    ClassDefinition(std::string name, Statement *body, Expression *superclass);
    ~ClassDefinition() override;
    std::string str() override;
};


#endif //INTERPRETER_STATEMENT_H
