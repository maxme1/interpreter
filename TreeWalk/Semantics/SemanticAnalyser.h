#ifndef INTERPRETER_SEMANTICANALYSIS_H
#define INTERPRETER_SEMANTICANALYSIS_H

#include "../../Parser/Statement/Statement.h"

class SemanticAnalyser : public TreeWalker {
    enum BlockType {
        Class, Loop, Function, Method
    };
    std::vector<std::map<std::string, bool>> scopes;
    std::vector<BlockType> types;

    void enterScope();
    void leaveScope();
    void setVariable(const std::string &name, bool value);
public:
    explicit SemanticAnalyser(std::vector<Statement *> statements);
    ~SemanticAnalyser();

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

    struct SyntaxError {
        std::string message;

        SyntaxError(const std::string &message, const Token &token) {
            this->message = message + " at " + std::to_string(token.line) + ":" + std::to_string(token.column);
        }
    };
};

#endif //INTERPRETER_SEMANTICANALYSIS_H
