#include "Statement.h"
#include "../Expression/Expression.h"

ExpressionStatement::ExpressionStatement(Expression *expression) : expression(expression) {}

std::string ExpressionStatement::str() {
    return expression->str() + ";";
}

ExpressionStatement::~ExpressionStatement() {
//    delete expression;
}

IfStatement::IfStatement(Expression *condition, Statement *left, Statement *right) : condition(condition), left(left),
                                                                                     right(right) {}

IfStatement::~IfStatement() {
//    delete condition, left, right;
}

std::string IfStatement::str() {
    std::string result = "if(" + condition->str() + ")";
    if (left)
        result += left->str();
    if (right)
        result += " else " + right->str();
    return result;
}

TryStatement::TryStatement(const std::vector<TryStatement::CatchStatement *> &catches, Statement *block) :
        catches(catches), block(block) {}

TryStatement::~TryStatement() {
//    delete block;
//    for (auto &&item : catches) {
//        delete item;
//    }
}

std::string TryStatement::str() {
    return "try - catch. i'm too lazy";
}

WhileStatement::WhileStatement(Expression *condition, Statement *body) : condition(condition), body(body) {}

WhileStatement::~WhileStatement() {
//    delete condition, body;
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

ReturnStatement::~ReturnStatement() {
//    delete expression;
}

std::string ReturnStatement::str() {
    std::string result = "return";
    if (expression)
        result += " " + expression->str();
    return result;
}

RaiseStatement::RaiseStatement(Expression *expression) : expression(expression) {}

RaiseStatement::~RaiseStatement() {
//    delete expression;
}

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
                                       Statement *body, bool unlimited) :
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

FunctionDefinition::~FunctionDefinition() {
//    delete body;
}

VariableDefinition::VariableDefinition(const std::string &name, Expression *assignee) : name(name), assignee(assignee) {

}

std::string VariableDefinition::str() {
    return  "var " + name + " = " + assignee->str();
}

ClassDefinition::ClassDefinition(std::string name, Block *body, Expression *superclass)
        : body(body), name(std::move(name)), superclass(superclass) {}

std::string ClassDefinition::str() {
    return "class " + name + body->str();
}

ClassDefinition::~ClassDefinition() {
//    delete body, superclass;
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

TryStatement::CatchStatement::~CatchStatement() {
//    for (auto argument : arguments)
//        delete argument;
//    delete block;
}
