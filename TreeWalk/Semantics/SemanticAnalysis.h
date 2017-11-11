#ifndef INTERPRETER_SEMANTICANALYSIS_H
#define INTERPRETER_SEMANTICANALYSIS_H

#include "../../Parser/Statement/Statement.h"

class SemanticAnalysis : public TreeWalker {
public:
    explicit SemanticAnalysis(std::vector<Statement *> statements);

    ObjPtr visit(Binary *expression) override;
    ObjPtr visit(Unary *expression) override;
    ObjPtr visit(Literal *expression) override;
    ObjPtr visit(SetVariable *expression) override;
    ObjPtr visit(SetAttribute *expression) override;
    ObjPtr visit(SetItem *expression) override;
    ObjPtr visit(Variable *expression) override;
    ObjPtr visit(FunctionExpression *expression) override;
    ObjPtr visit(GetAttribute *expression) override;
    ObjPtr visit(GetItem *expression) override;

    void visit(ExpressionStatement *statement) override;
    void visit(IfStatement *statement) override;
    void visit(TryStatement *statement) override;
    void visit(WhileStatement *statement) override;
    void visit(FunctionDefinition *statement) override;
    void visit(ClassDefinition *statement) override;
    void visit(ReturnStatement *statement) override;
    void visit(RaiseStatement *statement) override;
    void visit(ImportStatement *statement) override;
    void visit(ControlFlow *statement) override;
    void visit(Block *block) override;
};

#endif //INTERPRETER_SEMANTICANALYSIS_H
