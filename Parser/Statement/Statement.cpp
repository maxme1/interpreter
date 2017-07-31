#include "Statement.h"
#include "../Expression/Expression.h"

ExpressionStatement::ExpressionStatement(Expression *expression) : expression(expression) {}

std::string ExpressionStatement::str() {
    return expression->str() + ";";
}

ExpressionStatement::~ExpressionStatement() {
    delete expression;
}

IfStatement::IfStatement(Expression *condition, Statement *left, Statement *right) : condition(condition), left(left),
                                                                                     right(right) {}

IfStatement::~IfStatement() {
    delete condition, left, right;
}

std::string IfStatement::str() {
    std::string result = "if(" + condition->str() + ")";
    if (left)
        result += left->str();
    if (right)
        result += " else " + right->str();
    return result;
}

WhileStatement::WhileStatement(Expression *condition, Statement *body) : condition(condition), body(body) {}

WhileStatement::~WhileStatement() {
    delete condition, body;
}

std::string WhileStatement::str() {
    std::string result = "while(" + condition->str() + ")";
    if (body)
        result += body->str();
    return result;
}

ControlFlow::ControlFlow(Token::tokenType type, const std::string &body) : type(type), body(body) {}

std::string ControlFlow::str() { return body; }

ReturnStatement::ReturnStatement(Expression *expression) : expression(expression) {}

ReturnStatement::~ReturnStatement() { delete expression; }

std::string ReturnStatement::str() {
    std::string result = "return ";
    if (expression)
        result += expression->str();
    return result;
}

FunctionDefinition::FunctionDefinition(const std::string &name, const std::vector<std::string> &arguments,
                                       Statement *body) : body(body), arguments(arguments), name(name) {}

std::string FunctionDefinition::str() {
    std::string result = "def " + name + "(";
    bool first = true;
    for (auto &&argument : arguments) {
        if (!first)
            result += ", ";
        result += argument;
        first = false;
    }
    return result + ")" + body->str();
}

Block::Block(const std::vector<Statement *> &statements) : statements(statements) {}

std::string Block::str() {
    std::string result = "{\n";
    for (auto statement : statements) {
        result += "    " + statement->str() + "\n";
    }
    return result + "}\n";
}

Block::~Block() {
    statements.clear();
}