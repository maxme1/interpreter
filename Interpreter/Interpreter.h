#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <string>
#include <stack>
#include "../Tokenizer/Token.h"

class Binary;
class Unary;
class Literal;
class SetVariable;
class Variable;
class FunctionExpression;

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
    std::vector<Object *> scopes;
    std::stack<Object *> garbage;

    void addScope();
    void deleteScope();
    Object *getVariable(const std::string &name);
    void setVariable(const std::string &name, Object *value);
    void collect();
    void track(std::initializer_list<Object *> objects);
    void evaluateStatements(std::vector<Statement *> &statements);
public:
    Interpreter();
    void interpret(std::string text);

    Object *evaluate(Binary *expression);
    Object *evaluate(Unary *expression);
    Object *evaluate(Literal *expression);
    Object *evaluate(SetVariable *expression);
    Object *evaluate(Variable *expression);
    Object *evaluate(FunctionExpression *expression);

    void evaluate(ExpressionStatement *statement);
    void evaluate(IfStatement *statement);
    void evaluate(WhileStatement *statement);
    void evaluate(FunctionDefinition *statement);
    void evaluate(ReturnStatement *statement);
    void evaluate(ControlFlow *statement);
    void evaluate(Block *block);
};

#endif //INTERPRETER_INTERPRETER_H
