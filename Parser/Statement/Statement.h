#ifndef INTERPRETER_STATEMENT_H
#define INTERPRETER_STATEMENT_H


#include <string>
#include "../../Interpreter/Interpreter.h"
#include "../../Tokenizer/Token.h"

class Expression;

typedef std::shared_ptr<Expression> ExprPtr;

class Statement {
public:
    virtual void evaluate(Interpreter *interpreter) {
        interpreter->evaluate(this);
    };

    virtual std::string str() {
        return ";";
    };
};

typedef std::shared_ptr<Statement> StmtPtr;

class Block : public Statement {
    friend class Interpreter;

    std::vector<StmtPtr> statements;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    explicit Block(const std::vector<StmtPtr> &statements);

    std::string str() override;
};

class ExpressionStatement : public Statement {
    friend class Interpreter;

    ExprPtr expression;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    explicit ExpressionStatement(ExprPtr expression);

    std::string str() override;
};

class IfStatement : public Statement {
    friend class Interpreter;

    ExprPtr condition;
    StmtPtr left, right;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    explicit IfStatement(ExprPtr condition, StmtPtr left = nullptr, StmtPtr right = nullptr);

    std::string str() override;
};

struct TryStatement : public Statement {
    friend class Interpreter;

    struct CatchStatement {
        std::vector<ExprPtr> arguments;
        StmtPtr block;

        CatchStatement(std::vector<ExprPtr> &arguments, StmtPtr block) : block(block), arguments(arguments) {};
    };

    std::vector<std::shared_ptr<CatchStatement>> catches;

    StmtPtr block;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

    TryStatement(const std::vector<std::shared_ptr<CatchStatement>> catches, StmtPtr block) :
            catches(catches), block(block) {}

    std::string str() override;
};

class WhileStatement : public Statement {
    friend class Interpreter;

    ExprPtr condition;
    StmtPtr body;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    explicit WhileStatement(ExprPtr condition, StmtPtr body = nullptr);

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

    ExprPtr expression;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    explicit ReturnStatement(ExprPtr expression = nullptr);

    std::string str() override;
};

class RaiseStatement : public Statement {
    friend class Interpreter;

    ExprPtr expression;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    explicit RaiseStatement(ExprPtr expression);

    std::string str() override;
};

class ImportStatement : public Statement {
    friend class Interpreter;

    std::string path;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    explicit ImportStatement(std::string path);

    std::string str() override;
};

class FunctionDefinition : public Statement {
    friend class Interpreter;

    StmtPtr body;
    std::vector<std::string> arguments;
    std::string name;
    bool unlimited;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    FunctionDefinition(const std::string &name, const std::vector<std::string> &arguments, StmtPtr body,
                       bool unlimited);


    std::string str() override;
};

class ClassDefinition : public Statement {
    friend class Interpreter;

    StmtPtr body;
    ExprPtr superclass;
    std::string name;

    void evaluate(Interpreter *interpreter) override {
        interpreter->evaluate(this);
    }

public:
    ClassDefinition(std::string name, StmtPtr body, ExprPtr superclass);

    std::string str() override;
};


#endif //INTERPRETER_STATEMENT_H
