#include <iostream>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/native.h"

void Interpreter::evaluate(ReturnStatement *statement) {
    if (statement->expression)
        throw ReturnException(statement->expression->evaluate(this));
    throw ReturnException();
}

void Interpreter::evaluate(ExpressionStatement *statement) {
    statement->expression->evaluate(this);
}

void Interpreter::evaluate(IfStatement *statement) {
    auto cond = statement->condition->evaluate(this);
    if (cond->asBool()) {
        if (statement->left)
            statement->left->evaluate(this);
    } else if (statement->right)
        statement->right->evaluate(this);
}

void Interpreter::evaluate(WhileStatement *statement) {
    auto cond = statement->condition->evaluate(this);
    while (cond->asBool()) {
        try {
            if (statement->body)
                statement->body->evaluate(this);
        } catch (BreakException) {
            break;
        } catch (ContinueException) {}
        cond = statement->condition->evaluate(this);
    }
}

void Interpreter::evaluate(ControlFlow *statement) {
    if (statement->type == Token::CONTINUE)
        throw ContinueException();
    throw BreakException();
}

void Interpreter::evaluate(Block *block) {
    evaluateStatements(block->statements);
}

void Interpreter::evaluate(FunctionDefinition *statement) {
    setVariable(statement->name, new Callable(statement->arguments, statement->body));
}