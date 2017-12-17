#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <string>
#include <stack>
#include <utility>
#include <vector>
#include <cassert>
#include <iostream>
#include "../../Object/Scope.h"
#include "../TreeWalker.h"

class Object;
class Expression;
class Callable;
class Class;
class Interpreter : public TreeWalker {
public:
    Interpreter();
    ~Interpreter();
    void interpret(std::string text);
    bool interpretFile(const std::string &path);

    ObjPtr visit(Binary *expression) override;
    ObjPtr visit(Unary *expression) override;
    ObjPtr visit(Literal *expression) override;
    ObjPtr visit(SetVariable *expression) override;
    ObjPtr visit(SetAttribute *expression) override;
    ObjPtr visit(SetItem *expression) override;
    ObjPtr visit(Variable *expression) override;
    ObjPtr visit(CallExpression *expression) override;
    ObjPtr visit(SuperClass *expression) override;
    ObjPtr visit(GetAttribute *expression) override;
    ObjPtr visit(GetItem *expression) override;

    void visit(ExpressionStatement *statement) override;
    void visit(IfStatement *statement) override;
    void visit(TryStatement *statement) override;
    void visit(WhileStatement *statement) override;
    void visit(FunctionDefinition *statement) override;
    void visit(VariableDefinition *statement) override;
    void visit(ClassDefinition *statement) override;
    void visit(ReturnStatement *statement) override;
    void visit(RaiseStatement *statement) override;
    void visit(ImportStatement *statement) override;
    void visit(ControlFlow *statement) override;
    void visit(Block *block) override;

    void populate();
    void addFunction(const std::string &name, ObjPtr(*function)(const std::vector<ObjPtr> &, Interpreter *),
                     int argumentsCount, bool unlimited = false);

    std::vector<Scope::ptr> scopes;
    void enterScope();
    void addScope(std::shared_ptr<Scope> scope);
    void leaveScope();
    std::shared_ptr<Scope> getClosure();

    ObjPtr getVariable(const std::string &name, long level);
    void setVariable(const std::string &name, ObjPtr value, long level);
    void defineVariable(const std::string &name, ObjPtr value);

    ObjPtr callOperator(ObjPtr object, const std::vector<ObjPtr> &arguments);

    ObjPtr call(std::shared_ptr<Callable> callable, const std::vector<ObjPtr> &arguments);
    std::vector<ObjPtr> evaluateArguments(const std::vector<Expression *> &argsList);
    std::shared_ptr<Callable> getCallable(ObjPtr object);
    void checkArguments(std::shared_ptr<Callable> callable, long count);

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
