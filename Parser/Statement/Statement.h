#ifndef INTERPRETER_STATEMENT_H
#define INTERPRETER_STATEMENT_H

#include <string>
#include "../../TreeWalk/Interpreter/Interpreter.h"
#include "../../Tokenizer/Token.h"

class Expression;

struct Statement {
public:
    virtual void visit(TreeWalker *walker) = 0;
    virtual ~Statement() = default;
    virtual std::string str() = 0;
};

struct Block : public Statement {
    friend class Interpreter;

    std::vector<Statement *> statements;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

public:
    explicit Block(const std::vector<Statement *> &statements);
    ~Block() override;
    std::string str() override;
};

struct ExpressionStatement : public Statement {
    friend class Interpreter;

    Expression *expression;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

public:
    explicit ExpressionStatement(Expression *expression);
    ~ExpressionStatement() override;
    std::string str() override;
};

struct IfStatement : public Statement {
    friend class Interpreter;

    Expression *condition;
    Statement *left, *right;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

public:
    explicit IfStatement(Expression *condition, Statement *left = nullptr, Statement *right = nullptr);
    ~IfStatement() override;
    std::string str() override;
};

struct TryStatement : public Statement {
    friend class Interpreter;

    struct CatchStatement {
        std::vector<Expression *> arguments;
        Statement *block;

        CatchStatement(std::vector<Expression *> &arguments, Statement *block) : block(block), arguments(arguments) {};

        ~CatchStatement();
    };
    std::vector<CatchStatement *> catches;

    Statement *block;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

    TryStatement(const std::vector<CatchStatement *> &catches, Statement *block);
    ~TryStatement() override;
    std::string str() override;
};

struct WhileStatement : public Statement {
    friend class Interpreter;

    Expression *condition;
    Statement *body;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

public:
    explicit WhileStatement(Expression *condition, Statement *body = nullptr);
    ~WhileStatement() override;
    std::string str() override;
};

struct ControlFlow : public Statement {
    friend class Interpreter;
    Token::tokenType type;
    std::string body;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

public:
    ControlFlow(Token::tokenType type, const std::string &body);
    std::string str() override;
};

struct ReturnStatement : public Statement {
    friend class Interpreter;
    Expression *expression;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

public:
    explicit ReturnStatement(Expression *expression = nullptr);
    ~ReturnStatement() override;
    std::string str() override;
};

struct RaiseStatement : public Statement {
    friend class Interpreter;
    Expression *expression;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

public:
    explicit RaiseStatement(Expression *expression);
    ~RaiseStatement() override;
    std::string str() override;
};

class ImportStatement : public Statement {
    friend class Interpreter;
    std::string path;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

public:
    explicit ImportStatement(std::string path);
    std::string str() override;
};

struct FunctionDefinition : public Statement {
    friend class Interpreter;
    Statement *body;
    std::vector<std::string> arguments;
    std::string name;
    bool unlimited;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

public:
    FunctionDefinition(const std::string &name, const std::vector<std::string> &arguments, Statement *body,
                       bool unlimited);
    ~FunctionDefinition() override;
    std::string str() override;
};

struct VariableDefinition : public Statement {
    friend class Interpreter;
    Expression *assignee;
    std::string name;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

public:
    VariableDefinition(const std::string &name, Expression *assignee);
    std::string str() override;
};

struct ClassDefinition : public Statement {
    friend class Interpreter;
    Statement *body;
    Expression *superclass;
    std::string name;

    void visit(TreeWalker *walker) override {
        walker->visit(this);
    }

public:
    ClassDefinition(std::string name, Statement *body, Expression *superclass);
    ~ClassDefinition() override;
    std::string str() override;
};

#endif //INTERPRETER_STATEMENT_H
