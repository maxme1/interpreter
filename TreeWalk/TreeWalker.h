#ifndef INTERPRETER_TREEWALKER_H
#define INTERPRETER_TREEWALKER_H

#include "../Object/Object.h"

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
class VariableDefinition;
class ClassDefinition;
class ReturnStatement;
class RaiseStatement;
class ImportStatement;
class ControlFlow;
class Block;

struct TreeWalker {
public:
    virtual ObjPtr visit(Binary *expression)= 0;
    virtual ObjPtr visit(Unary *expression)= 0;
    virtual ObjPtr visit(Literal *expression)=0;
    virtual ObjPtr visit(SetVariable *expression)=0;
    virtual ObjPtr visit(SetAttribute *expression)=0;
    virtual ObjPtr visit(SetItem *expression)=0;
    virtual ObjPtr visit(Variable *expression)=0;
    virtual ObjPtr visit(FunctionExpression *expression)=0;
    virtual ObjPtr visit(GetAttribute *expression)=0;
    virtual ObjPtr visit(GetItem *expression)=0;

    virtual void visit(ExpressionStatement *statement)=0;
    virtual void visit(IfStatement *statement)=0;
    virtual void visit(TryStatement *statement)=0;
    virtual void visit(WhileStatement *statement)=0;
    virtual void visit(FunctionDefinition *statement)=0;
    virtual void visit(VariableDefinition *statement)=0;
    virtual void visit(ClassDefinition *statement)=0;
    virtual void visit(ReturnStatement *statement)=0;
    virtual void visit(RaiseStatement *statement)=0;
    virtual void visit(ImportStatement *statement)=0;
    virtual void visit(ControlFlow *statement)=0;
    virtual void visit(Block *block)=0;

    void visitStatements(std::vector<Statement *> &statements);
};

#endif //INTERPRETER_TREEWALKER_H
