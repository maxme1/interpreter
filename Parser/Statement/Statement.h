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
    virtual std::string str() = 0;
    typedef shared(Statement) ptr;
};

struct Block : public Statement {
    friend class Interpreter;

    std::vector<shared(Statement) > statements;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit Block(Token token, const std::vector<shared(Statement) > &statements);
    std::string str() override;
};

struct ExpressionStatement : public Statement {
    friend class Interpreter;

    shared(Expression) expression;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit ExpressionStatement(shared(Expression) expression);
    std::string str() override;
};

struct IfStatement : public Statement {
    friend class Interpreter;

    shared(Expression) condition;
    shared(Statement) left, right;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit IfStatement(Token token, shared(Expression) condition, shared(Statement) left = nullptr,
                         shared(Statement) right = nullptr);
    std::string str() override;
};

struct TryStatement : public Statement {
    friend class Interpreter;

    struct CatchStatement {
        std::vector<shared(Expression) > arguments;
        shared(Statement) block;

        CatchStatement(std::vector<shared(Expression) > &arguments, shared(Statement) block) : block(block),
                                                                                               arguments(arguments) {};
    };
    std::vector<shared(CatchStatement) > catches;

    shared(Statement) block;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

    TryStatement(const std::vector<shared(CatchStatement) > &catches, shared(Statement) block);
    std::string str() override;
};

struct WhileStatement : public Statement {
    friend class Interpreter;

    shared(Expression) condition;
    shared(Statement) body;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit WhileStatement(shared(Expression) condition, shared(Statement) body = nullptr);
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
    shared(Expression) expression;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit ReturnStatement(Token token, shared(Expression) expression = nullptr);
    std::string str() override;
};

struct RaiseStatement : public Statement {
    friend class Interpreter;
    shared(Expression) expression;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    explicit RaiseStatement(shared(Expression) expression);
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
    struct Argument {
        std::string name;
        shared(Expression) defaultValue;
        bool positional, variable;

        Argument(const std::string &name, shared(Expression) defaultValue, bool positional, bool variable) :
                name(name), defaultValue(defaultValue), positional(positional), variable(variable) {}
    };

    std::string name;
    shared(Statement) body;
    std::vector<Argument> arguments;

    void visit(TreeWalker *walker) override {
        try {
            walker->visit(this);
        } catch (BaseExceptionWrapper &e) {
            e.push(this->token);
            throw;
        }
    }

public:
    FunctionDefinition(const std::string &name, shared(Statement) body, const std::vector<Argument> &arguments);
    std::string str() override;
};

struct VariableDefinition : public Statement {
    friend class Interpreter;
    shared(Expression) assignee;
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
    VariableDefinition(const std::string &name, shared(Expression) assignee);
    std::string str() override;
};

struct ClassDefinition : public Statement {
    friend class Interpreter;
    shared(Block) body;
    shared(Expression) superclass;
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
    ClassDefinition(std::string name, shared(Block) body, shared(Expression) superclass);
    std::string str() override;
};

#endif //INTERPRETER_STATEMENT_H
