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

IfStatement::IfStatement(Expression *condition, Statement *left, Statement *right) : condition(condition), left(left),
                                                                                     right(right) {}

IfStatement::~IfStatement() {
    delete condition, left, right;
}

void IfStatement::evaluate(Interpreter *interpreter) {
    interpreter->evaluate(this);
}

std::string IfStatement::str() {
    std::string result = "if(" + condition->str() + ")";
    if (left)
        result += left->str();
    if (right)
        result += right->str();
    return result;
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