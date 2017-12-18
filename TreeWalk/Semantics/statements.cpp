#include <iostream>
#include "SemanticAnalyser.h"
#include "../../Parser/Parser.h"

void SemanticAnalyser::visit(Block *block) {
    enterScope();
    visitStatements(block->statements);
    leaveScope();
}

void SemanticAnalyser::visit(VariableDefinition *statement) {
//    TODO: throw if defined?
    setVariable(statement->name, false);
    if (statement->assignee)
        statement->assignee->visit(this);
    setVariable(statement->name, true);
}

void SemanticAnalyser::visit(FunctionDefinition *statement) {
    setVariable(statement->name, true);
    if (!types.empty() and types.back() == BlockType::Class)
        types.push_back(BlockType::Method);
    else
        types.push_back(BlockType::Function);
    statement->body->visit(this);
    types.pop_back();
}

void SemanticAnalyser::visit(ClassDefinition *statement) {
    setVariable(statement->name, true);

    types.push_back(BlockType::Class);
    if (statement->superclass)
        statement->superclass->visit(this);

    enterScope();
    if (statement->superclass)
        setVariable("super", true);
    visitStatements(statement->body->statements);

    leaveScope();
    types.pop_back();
}

void SemanticAnalyser::visit(ReturnStatement *statement) {
    if (types.empty() or (types.back() != BlockType::Function and types.back() != BlockType::Method))
        throw SyntaxError("Return outside function or method", statement->token);

    if (statement->expression)
        statement->expression->visit(this);
}

void SemanticAnalyser::visit(RaiseStatement *statement) {
    if (statement->expression)
        statement->expression->visit(this);
}

void SemanticAnalyser::visit(ImportStatement *statement) {
//    TODO: implement
}

void SemanticAnalyser::visit(ExpressionStatement *statement) {
    statement->expression->visit(this);
}

void SemanticAnalyser::visit(IfStatement *statement) {
    statement->condition->visit(this);
    if (statement->left)
        statement->left->visit(this);
    if (statement->right)
        statement->right->visit(this);
}

void SemanticAnalyser::visit(TryStatement *statement) {
//    TODO: implement
}

void SemanticAnalyser::visit(WhileStatement *statement) {
    statement->condition->visit(this);
    if (statement->body) {
        types.push_back(BlockType::Loop);
        statement->body->visit(this);
        types.pop_back();
    }
}

void SemanticAnalyser::visit(ControlFlow *statement) {
    if (types.empty() or types.back() != BlockType::Loop)
        throw SyntaxError("Control flow outside loop", statement->token);
}
