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
class Instance;
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

    void addFunction(const std::string &name, ObjPtr(*function)(Interpreter *, const std::vector<ObjPtr> &,
                                                                const std::map<std::string, ObjPtr> &),
                     int argumentsCount = 0);

    std::vector<Scope::ptr> scopes;
    void enterScope();
    void addScope(std::shared_ptr<Scope> scope);
    void leaveScope();
    std::shared_ptr<Scope> getClosure();

    ObjPtr getVariable(const std::string &name, long level);
    void setVariable(const std::string &name, ObjPtr value, long level);
    void defineVariable(const std::string &name, ObjPtr value);

    ObjPtr call(ObjPtr object, const std::vector<ObjPtr> &positional,
                const std::map<std::string, ObjPtr> &keyword = std::map<std::string, ObjPtr>());
    std::vector<ObjPtr> evaluateArguments(const std::vector<Expression *> &argsList);
    std::shared_ptr<Callable> getCallable(ObjPtr object);

    static bool isDerived(ObjPtr derived, ObjPtr base);
    static bool isInstance(ObjPtr instance, ObjPtr base);
//    Exceptions
    struct BaseException {
    };

    struct ExceptionWrapper : BaseException {
        std::shared_ptr<Instance> exception;
        explicit ExceptionWrapper(Object *exception);
        explicit ExceptionWrapper(ObjPtr exception);
    };

    struct ReturnException : BaseException {
        ObjPtr content;

        explicit ReturnException(ObjPtr content = nullptr) : content(content) {}
    };
    struct ContinueException : BaseException {
    };
    struct BreakException : BaseException {
    };
};

#endif //INTERPRETER_INTERPRETER_H
