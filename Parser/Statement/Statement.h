#ifndef INTERPRETER_STATEMENT_H
#define INTERPRETER_STATEMENT_H

#include <string>
#include "../../TreeWalk/Interpreter/Interpreter.h"
#include "../../Tokenizer/Token.h"
#include "../../ExceptionWrapper.h"

class Expression;

struct Statement {
    Token token;
public:
    explicit Statement(Token token) : token(token) {};

    virtual void visit(TreeWalker *walker) = 0;
    virtual ~Statement() = default;
    virtual std::string str() = 0;
};

struct Block : public Statement {
    friend class Interpreter;

    std::vector<Statement *> statements;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit Block(Token token, const std::vector<Statement *> &statements);
    ~Block() override;
    std::string str() override;
};

struct ExpressionStatement : public Statement {
    friend class Interpreter;

    Expression *expression;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
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
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit IfStatement(Token token, Expression *condition, Statement *left = nullptr, Statement *right = nullptr);
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
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
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
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit WhileStatement(Expression *condition, Statement *body = nullptr);
    ~WhileStatement() override;
    std::string str() override;
};

struct ControlFlow : public Statement {
    friend class Interpreter;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    ControlFlow(Token token);
    std::string str() override;
};

struct ReturnStatement : public Statement {
    friend class Interpreter;
    Expression *expression;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit ReturnStatement(Token token, Expression *expression = nullptr);
    ~ReturnStatement() override;
    std::string str() override;
};

struct RaiseStatement : public Statement {
    friend class Interpreter;
    Expression *expression;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
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
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit ImportStatement(Token token, std::string path);
    std::string str() override;
};

struct FunctionDefinition : public Statement {
    friend class Interpreter;
    Statement *body;
    std::vector<std::string> arguments;
    std::map<std::string, Expression *> defaults;
    std::string name;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    FunctionDefinition(const std::string &name, Statement *body, const std::vector<std::string> &arguments,
                       const std::vector<SetVariable *> &kwargs);
    ~FunctionDefinition() override;
    std::string str() override;
};

struct VariableDefinition : public Statement {
    friend class Interpreter;
    Expression *assignee;
    std::string name;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    VariableDefinition(const std::string &name, Expression *assignee);
    std::string str() override;
};

struct ClassDefinition : public Statement {
    friend class Interpreter;
    Block *body;
    Expression *superclass;
    std::string name;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    ClassDefinition(std::string name, Block *body, Expression *superclass);
    ~ClassDefinition() override;
    std::string str() override;
};

#endif //INTERPRETER_STATEMENT_H
