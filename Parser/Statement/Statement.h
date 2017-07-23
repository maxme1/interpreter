#ifndef INTERPRETER_STATEMENT_H
#define INTERPRETER_STATEMENT_H


#include <string>
#include "../../Interpreter/Interpreter.h"

class Expression;

class Statement {
//    friend class Interpreter;

//    std::string body;
protected:
//    Token::tokenType type;
public:
//    Statement(const std::string &body, Token::tokenType type);

    virtual std::string str() = 0; // { return ""; };

    virtual void evaluate(Interpreter *interpreter) = 0;
};

class ExpressionStatement : public Statement {
    friend class Interpreter;

    Expression *expression;

public:
    ExpressionStatement(Expression *expression);
    void evaluate(Interpreter *interpreter) override;
    std::string str() override;
};

#endif //INTERPRETER_STATEMENT_H
