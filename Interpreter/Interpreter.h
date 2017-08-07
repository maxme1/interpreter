#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <string>
#include <stack>
#include <vector>
#include "../Object/Types/Scope.h"

class Expression;
class Binary;
class Unary;
class Literal;
class SetVariable;
class SetAttribute;
class SetItem;
class Variable;
class FunctionExpression;
class GetAttribute;
class GetItem;

class Statement;
class ExpressionStatement;
class IfStatement;
class WhileStatement;
class FunctionDefinition;
class ClassDefinition;
class ReturnStatement;
class ControlFlow;
class Block;

class Object;
class Callable;
class API;
class Interpreter {
    friend class API;
public:
    Interpreter();
    ~Interpreter();
    void interpret(std::string text);

    Object *evaluate(Binary *expression);
    Object *evaluate(Unary *expression);
    Object *evaluate(Literal *expression);
    Object *evaluate(SetVariable *expression);
    Object *evaluate(SetAttribute *expression);
    Object *evaluate(SetItem *expression);
    Object *evaluate(Variable *expression);
    Object *evaluate(FunctionExpression *expression);
    Object *evaluate(GetAttribute *expression);
    Object *evaluate(GetItem *expression);

    void evaluate(ExpressionStatement *statement);
    void evaluate(IfStatement *statement);
    void evaluate(WhileStatement *statement);
    void evaluate(FunctionDefinition *statement);
    void evaluate(ClassDefinition *statement);
    void evaluate(ReturnStatement *statement);
    void evaluate(ControlFlow *statement);
    void evaluate(Block *block);
private:
    API *api;

    Scope *scope = nullptr;
    std::stack<Object *> garbage;

    void addScope(Scope *ready = nullptr);
    void deleteScope();
    void collect();
    Object *track(Object *object);

    Object *getVariable(const std::string &name);
    void setVariable(const std::string &name, Object *value);
    void evaluateStatements(std::vector<Statement *> &statements);

    Object *callOperator(Object *object, const std::vector<Object *> &arguments);
    Object *callFunction(Object *object, const std::vector<Expression *> &argsList);

    Object *call(Callable *callable, const std::vector<Object *> &arguments);
    Callable *getCallable(Object *object);
    void checkArguments(Callable *callable, int count);

//    Control Flow
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
