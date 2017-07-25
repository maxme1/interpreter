#include "Statement.h"
#include "../Expression/Expression.h"

ExpressionStatement::ExpressionStatement(Expression *expression) : expression(expression) {}

std::string ExpressionStatement::str() {
    return expression->str() + ";";
}

void ExpressionStatement::evaluate(Interpreter *interpreter) {
    interpreter->evaluate(this);
}

ExpressionStatement::~ExpressionStatement() {
    delete expression;
}

Block::Block(const std::vector<Statement *> &statements) : statements(statements) {}

std::string Block::str() {
    std::string result = "{\n";
    for (auto statement : statements) {
        result += "    " + statement->str() + "\n";
    }
    return result + "}\n";
}

void Block::evaluate(Interpreter *interpreter) {
    interpreter->evaluate(this);
}

Block::~Block() {
    statements.clear();
}