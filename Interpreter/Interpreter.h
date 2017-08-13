#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <string>
#include <stack>
#include <utility>
#include <vector>
#include <cassert>
#include <iostream>
#include "../Object/Scope.h"

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
class TryStatement;
class WhileStatement;
class FunctionDefinition;
class ClassDefinition;
class ReturnStatement;
class RaiseStatement;
class ImportStatement;
class ControlFlow;
class Block;

typedef std::shared_ptr<Expression> ExprPtr;

class Object;
class Callable;
class Class;
class API;
class Interpreter {
    friend class API;
public:
    Interpreter();
    ~Interpreter();
    void interpret(std::string text);
    bool interpretFile(const std::string &path);

    ObjPtr evaluate(Binary *expression);
    ObjPtr evaluate(Unary *expression);
    ObjPtr evaluate(Literal *expression);
    ObjPtr evaluate(SetVariable *expression);
    ObjPtr evaluate(SetAttribute *expression);
    ObjPtr evaluate(SetItem *expression);
    ObjPtr evaluate(Variable *expression);
    ObjPtr evaluate(FunctionExpression *ExprPtr );
    ObjPtr evaluate(GetAttribute *ExprPtr );
    ObjPtr evaluate(GetItem *ExprPtr );

    void evaluate(Statement *statement);
    void evaluate(ExpressionStatement *statement);
    void evaluate(IfStatement *statement);
    void evaluate(TryStatement *statement);
    void evaluate(WhileStatement *statement);
    void evaluate(FunctionDefinition *statement);
    void evaluate(ClassDefinition *statement);
    void evaluate(ReturnStatement *statement);
    void evaluate(RaiseStatement *statement);
    void evaluate(ImportStatement *statement);
    void evaluate(ControlFlow *statement);
    void evaluate(Block *block);
private:
    const int textRange = 50;

    API *api;
    void populate();
    void addFunction(const std::string &name, ObjPtr(*function)(const std::vector<ObjPtr> &, API *),
                     int argumentsCount = 0, bool unlimited = false);

    std::vector<std::shared_ptr<Scope> > scopes;
    void addScope(std::shared_ptr<Scope> context = nullptr);
    void deleteScope();
    std::shared_ptr<Scope> getContext();

    ObjPtr getVariable(const std::string &name);
    void setVariable(const std::string &name, ObjPtr value);
    void evaluateStatements(std::vector<std::shared_ptr<Statement>> &statements);

    ObjPtr callOperator(ObjPtr object, const std::vector<ObjPtr> &arguments);
    ObjPtr callFunction(ObjPtr object, const std::vector<ExprPtr> &argsList);

    ObjPtr call(std::shared_ptr<Callable> callable, const std::vector<ObjPtr> &arguments);
    std::vector<ObjPtr> evaluateArguments(const std::vector<ExprPtr> &argsList);
    std::shared_ptr<Callable> getCallable(ObjPtr object);
    void checkArguments(std::shared_ptr<Callable> callable, int count);

    static bool isDerived(ObjPtr derived, std::shared_ptr<Class> base);

//    Exceptions
    struct ExceptionWrapper {
        ObjPtr exception;
        explicit ExceptionWrapper(Object *exception);
        explicit ExceptionWrapper(const ObjPtr &exception);
    };
    struct ReturnException {
        ObjPtr content;

        explicit ReturnException(ObjPtr content = nullptr) : content(std::move(content)) {}
    };
    struct FlowException {
    };
    struct ContinueException : FlowException {
    };
    struct BreakException : FlowException {
    };
};

#endif //INTERPRETER_INTERPRETER_H
