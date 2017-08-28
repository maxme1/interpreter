#include "Statement.h"
#include "../Expression/Expression.h"

ExpressionStatement::ExpressionStatement(ExprPtr expression) : expression(expression) {}

std::string ExpressionStatement::str() {
    return expression->str() + ";";
}

IfStatement::IfStatement(ExprPtr condition, StmtPtr left, StmtPtr right) : condition(condition), left(left),
                                                                           right(right) {}

std::string IfStatement::str() {
    std::string result = "if(" + condition->str() + ")";
    if (left)
        result += left->str();
    if (right)
        result += " else " + right->str();
    return result;
}

std::string TryStatement::str() {
    return "try - catch. i'm too lazy";
}

WhileStatement::WhileStatement(ExprPtr condition, StmtPtr body) : condition(condition), body(body) {}

std::string WhileStatement::str() {
    std::string result = "while(" + condition->str() + ")";
    if (body)
        result += body->str();
    return result;
}

ForStatement::ForStatement(Token variable, ExprPtr target, StmtPtr body) :
        variable(variable), target(target), body(body) {}

std::string ForStatement::str() {
    std::string result = "for " + variable.body + " in " + target->str();
    if (body)
        result += body->str();
    return result;
}

ControlFlow::ControlFlow(Token::tokenType type, const std::string &body) : type(type), body(body) {}

std::string ControlFlow::str() { return body; }

ReturnStatement::ReturnStatement(ExprPtr expression) : expression(expression) {}

std::string ReturnStatement::str() {
    std::string result = "return";
    if (expression)
        result += " " + expression->str();
    return result;
}

RaiseStatement::RaiseStatement(ExprPtr expression) : expression(expression) {}

std::string RaiseStatement::str() {
    std::string result = "raise";
    if (expression)
        result += " " + expression->str();
    return result;
}

ImportStatement::ImportStatement(std::string path) : path(std::move(path)) {}

std::string ImportStatement::str() {
    return "import " + path;
}

FunctionDefinition::FunctionDefinition(const std::string &name, const std::vector<std::string> &arguments,
                                       StmtPtr body, bool unlimited) :
        body(body), arguments(arguments), name(name), unlimited(unlimited) {}

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

ClassDefinition::ClassDefinition(std::string name, StmtPtr body, ExprPtr superclass)
        : body(body), name(std::move(name)), superclass(superclass) {}

std::string ClassDefinition::str() {
    return "class " + name + body->str();
}

Block::Block(const std::vector<StmtPtr> &statements) : statements(statements) {}

std::string Block::str() {
    std::string result = "{\n";
    for (auto statement : statements) {
        result += "    " + statement->str() + "\n";
    }
    return result + "}\n";
}
