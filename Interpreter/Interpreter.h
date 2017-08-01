#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <string>
#include <stack>
#include <vector>
#include "../Tokenizer/Token.h"

class Binary;
class Unary;
class Literal;
class SetVariable;
class SetAttribute;
class Variable;
class FunctionExpression;
class GetAttribute;

class Statement;
class ExpressionStatement;
class IfStatement;
class WhileStatement;
class FunctionDefinition;
class ReturnStatement;
class ControlFlow;
class Block;

class Object;
class Interpreter {
public:
    Interpreter();
    ~Interpreter();
    void interpret(std::string text);

    Object *evaluate(Binary *expression);
    Object *evaluate(Unary *expression);
    Object *evaluate(Literal *expression);
    Object *evaluate(SetVariable *expression);
    Object *evaluate(SetAttribute *expression);
    Object *evaluate(Variable *expression);
    Object *evaluate(FunctionExpression *expression);
    Object *evaluate(GetAttribute *expression);

    void evaluate(ExpressionStatement *statement);
    void evaluate(IfStatement *statement);
    void evaluate(WhileStatement *statement);
    void evaluate(FunctionDefinition *statement);
    void evaluate(ReturnStatement *statement);
    void evaluate(ControlFlow *statement);
    void evaluate(Block *block);
private:
    std::vector<Object *> scopes;
    std::stack<Object *> garbage;

    void addScope();
    void deleteScope();
    void collect();
    Object *track(Object *object);
    Object *getVariable(const std::string &name);
    void setVariable(const std::string &name, Object *value);
    void evaluateStatements(std::vector<Statement *> &statements);

    struct ReturnException {
        Object *content;

        explicit ReturnException(Object *content = nullptr) : content(content) {}
    };
    struct FlowException {
    };
    struct ContinueException : FlowException {
    };
    struct BreakException : FlowException {
    };
};

#endif //INTERPRETER_INTERPRETER_H
