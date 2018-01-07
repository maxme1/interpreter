#include "Statement.h"
#include "../Expression/Expression.h"

ExpressionStatement::ExpressionStatement(shared(Expression) expression) :
        Statement(expression->token), expression(expression) {}

std::string ExpressionStatement::str() {
    return expression->str() + ";";
}

IfStatement::IfStatement(Token token, shared(Expression) condition, shared(Statement) left, shared(Statement) right) :
        Statement(token), condition(condition), left(left), right(right) {}

std::string IfStatement::str() {
    std::string result = "if(" + condition->str() + ")";
    if (left)
        result += left->str();
    if (right)
        result += " else " + right->str();
    return result;
}

TryStatement::TryStatement(const std::vector<shared(TryStatement::CatchStatement) > &catches, shared(Statement) block) :
        Statement(block->token), catches(catches), block(block) {}

std::string TryStatement::str() {
    return "try - catch. i'm too lazy";
}

WhileStatement::WhileStatement(shared(Expression) condition, shared(Statement) body) :
        Statement(body->token), condition(condition), body(body) {}

std::string WhileStatement::str() {
    std::string result = "while(" + condition->str() + ")";
    if (body)
        result += body->str();
    return result;
}

ControlFlow::ControlFlow(Token token) : Statement(token) {}

std::string ControlFlow::str() { return token.body; }

ReturnStatement::ReturnStatement(Token token, shared(Expression) expression) :
        Statement(token), expression(expression) {}

std::string ReturnStatement::str() {
    std::string result = "return";
    if (expression)
        result += " " + expression->str();
    return result;
}

RaiseStatement::RaiseStatement(shared(Expression) expression) : Statement(expression->token), expression(expression) {}

std::string RaiseStatement::str() {
    std::string result = "raise";
    if (expression)
        result += " " + expression->str();
    return result;
}

ImportStatement::ImportStatement(Token token, std::string path) : Statement(token), path(std::move(path)) {}

std::string ImportStatement::str() {
    return "import " + path;
}

//TODO provide proper tokens
FunctionDefinition::FunctionDefinition(const std::string &name, shared(Statement) body,
                                       const std::vector<Argument> &arguments) :
        Statement(body->token), body(body), arguments(arguments), name(name) {}

std::string FunctionDefinition::str() {
    std::string result = "def " + name + "(";
    bool first = true;
    for (auto &&argument : arguments) {
        if (!first)
            result += ", ";
//        result += argument;
        first = false;
    }
    return result + ")" + body->str();
}

VariableDefinition::VariableDefinition(const std::string &name, shared(Expression) assignee) :
        Statement(assignee->token), name(name), assignee(assignee) {}

std::string VariableDefinition::str() {
    return "var " + name + " = " + assignee->str();
}

ClassDefinition::ClassDefinition(std::string name, shared(Block) body, shared(Expression) superclass)
        : Statement(body->token), body(body), name(std::move(name)), superclass(superclass) {}

std::string ClassDefinition::str() {
    return "class " + name + body->str();
}

Block::Block(Token token, const std::vector<shared(Statement) > &statements) : Statement(token),
                                                                               statements(statements) {}

std::string Block::str() {
    std::string result = "{\n";
    for (auto statement : statements) {
        result += "    " + statement->str() + "\n";
    }
    return result + "}\n";
}
