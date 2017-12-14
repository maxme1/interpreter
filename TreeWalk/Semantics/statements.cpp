#include <iostream>
#include "SemanticAnalyser.h"
#include "../../Parser/Parser.h"

void SemanticAnalysis::visit(Block *block) {
    enterScope();
    visitStatements(block->statements);
    leaveScope();
}

void SemanticAnalysis::visit(VariableDefinition *statement) {
//    TODO: throw if defined?
    setVariable(statement->name, false);
    if (statement->assignee)
        statement->assignee->visit(this);
    setVariable(statement->name, true);
}

void SemanticAnalysis::visit(FunctionDefinition *statement) {
    setVariable(statement->name, true);
    statement->body->visit(this);
}

void SemanticAnalysis::visit(ClassDefinition *statement) {
    setVariable(statement->name, true);

    if (statement->superclass)
        statement->superclass->visit(this);

    statement->body->visit(this);
}

void SemanticAnalysis::visit(ReturnStatement *statement) {
    if (statement->expression)
        statement->expression->visit(this);
}

void SemanticAnalysis::visit(RaiseStatement *statement) {
    if (statement->expression)
        statement->expression->visit(this);
}

void SemanticAnalysis::visit(ImportStatement *statement) {}

void SemanticAnalysis::visit(ExpressionStatement *statement) {
    statement->expression->visit(this);
}

void SemanticAnalysis::visit(IfStatement *statement) {
    statement->condition->visit(this);
    if (statement->left)
        statement->left->visit(this);
    if (statement->right)
        statement->right->visit(this);
}

void SemanticAnalysis::visit(TryStatement *statement) {
//    TODO: implement
}

void SemanticAnalysis::visit(WhileStatement *statement) {
    statement->condition->visit(this);
    if (statement->body)
        statement->body->visit(this);
}

void SemanticAnalysis::visit(ControlFlow *statement) {
//    TODO: implement
}
