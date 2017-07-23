#include "Statement.h"
#include "../Expression/Expression.h"

//Statement::Statement(const std::string &body, Token::tokenType type) : body(body), type(type) {}
//
ExpressionStatement::ExpressionStatement(Expression *expression) : expression(expression) {}

std::string ExpressionStatement::str() {
    return "(Expression " + expression->str() + ")";
}

void ExpressionStatement::evaluate(Interpreter *interpreter) {
    interpreter->evaluate(this);
}
