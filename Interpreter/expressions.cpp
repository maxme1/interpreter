#include <iostream>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/Types/Int.h"
#include "../Object/native.h"

Object *Interpreter::evaluate(Binary *expression) {
    Object *left = expression->left->evaluate(this), *right = expression->right->evaluate(this);
    if (expression->type == Token::ADD)
        return track(left->add(right));
    if (expression->type == Token::SUB)
        return track(left->subtract(right));
    if (expression->type == Token::MUL)
        return track(left->multiply(right));
    if (expression->type == Token::DIV)
        return track(left->divide(right));
//    comparison
    if (expression->type == Token::EQUAL)
        return track(left->equal(right));
    if (expression->type == Token::NOT_EQUAL)
        return track(left->not_equal(right));
    if (expression->type == Token::GREATER_OR_EQUAL)
        return track(left->greater_or_equal(right));
    if (expression->type == Token::GREATER)
        return track(left->greater(right));
    if (expression->type == Token::LESS)
        return track(left->less(right));
    if (expression->type == Token::LESS_OR_EQUAL)
        return track(left->less_or_equal(right));
    return nullptr;
}

Object *Interpreter::evaluate(Unary *expression) {
    Object *argument = expression->argument->evaluate(this);
    if (expression->type == Token::ADD)
        return track(argument->unary_add());
    if (expression->type == Token::SUB)
        return track(argument->unary_subtract());
    if (expression->type == Token::BRACKET)
        return argument;
    return nullptr;
}

Object *Interpreter::evaluate(Literal *expression) {
    auto body = expression->body;
    auto type = expression->type;
    if (type == Token::NUMBER)
        return track(new Int(std::atoi(body.c_str())));
    if (type == Token::BOOL)
        return track(new Bool(body == "True"));
    if (type == Token::NONE)
        return track(new None());
    return nullptr;
}

Object *Interpreter::evaluate(SetVariable *expression) {
    auto value = expression->value->evaluate(this);
    setVariable(expression->name, value);
    return value;
}

Object *Interpreter::evaluate(Variable *expression) {
//    leaks here?
    return getVariable(expression->body);
}

//TODO: closures don't work for now
Object *Interpreter::evaluate(FunctionExpression *expression) {
    Object *obj = expression->target->evaluate(this);
    Callable *callable = dynamic_cast<Callable *>(obj);
    if (!callable)
        throw Exception("Object is not callable");

    if (expression->argsList.size() != callable->arguments.size())
        throw Exception("Number of arguments doesn't match");

    addScope();
    for (int i = 0; i < callable->arguments.size(); i++) {
        auto arg = expression->argsList[i]->evaluate(this);
        setVariable(callable->arguments[i], arg);
    }
    Object *returnObject = nullptr;
    try {
        if (callable->body)
            callable->body->evaluate(this);
        else
            returnObject = callable->call(scopes.back());
    } catch (ReturnException &e) {
        returnObject = e.content;
    } catch (FlowException &e) {
//        TODO: move to syntactic errors
        throw Exception("Control flow outside loop");
    } catch (Exception &e) {
        deleteScope();
        throw e;
    }
    if (!returnObject)
        returnObject = new None;
    track(returnObject);
    deleteScope();
    return returnObject;
}