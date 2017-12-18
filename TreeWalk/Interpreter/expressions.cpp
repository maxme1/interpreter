#include <iostream>
#include "Interpreter.h"
#include "../../Parser/Parser.h"
#include "../../Object/Types/Int.h"
#include "../../Object/Types/None.h"
#include "../../Object/Native/Native.h"

std::map<Token::tokenType, std::string> binary = {
        {Token::ADD,              "add"},
        {Token::SUB,              "sub"},
        {Token::MUL,              "mul"},
        {Token::DIV,              "div"},
        {Token::EQUAL,            "eq"},
        {Token::GREATER,          "gr"},
        {Token::GREATER_OR_EQUAL, "geq"},
        {Token::LESS,             "ls"},
        {Token::LESS_OR_EQUAL,    "leq"},
        {Token::NOT_EQUAL,        "neq"}
};

std::map<Token::tokenType, std::string> unary = {
        {Token::ADD, "uadd"},
        {Token::SUB, "usub"},
};

ObjPtr Interpreter::visit(Binary *expression) {
    auto left = expression->left->visit(this), right = expression->right->visit(this);
    auto name = binary.find(expression->token.type);
    if (name != binary.end()) {
        auto method = left->findAttribute(name->second);
        if (method)
            return callOperator(method, {right});
    }

    assert(false);
    return nullptr;

//    default behavior
//    if (expression->ofType(Token::EQUAL))
//        return New(Bool(left == right));
//    if (expression->ofType(Token::NOT_EQUAL))
//        return New(Bool(left != right));
//
//    throw Wrap(new Exception("Operator not defined"));
}

ObjPtr Interpreter::visit(Unary *expression) {
    ObjPtr argument = expression->argument->visit(this);
    auto name = unary.find(expression->token.type);
    if (name != unary.end()) {
        auto method = argument->findAttribute(name->second);
        if (method)
            return callOperator(method, {});
    }
    assert(false);
    return nullptr;
//    throw Wrap(new Exception("Operator not defined"));
}

ObjPtr Interpreter::visit(Literal *expression) {
    if (expression->ofType(Token::NUMBER))
        return New(Int(std::stoi(expression->token.body)));
    if (expression->ofType(Token::BOOL))
        return New(Bool(expression->token.body == "True"));
    if (expression->ofType(Token::NONE))
        return New(None());
    if (expression->ofType(Token::STRING)) {
        auto body = expression->token.body;
        return New(String(body.substr(1, body.size() - 2)));
    }
    assert(false);
    return nullptr;
}

ObjPtr Interpreter::visit(SetVariable *expression) {
    auto value = expression->value->visit(this);
    setVariable(expression->name, value, expression->level);
    return value;
}

ObjPtr Interpreter::visit(SetAttribute *expression) {
//    TODO: also looks bad
    auto value = expression->value->visit(this), target = expression->target->target->visit(this);
    target->setAttribute(expression->target->name, value);
    return value;
}

ObjPtr Interpreter::visit(SetItem *expression) {
//    TODO: also looks bad
    auto target = expression->target->target->visit(this);
    auto argument = expression->target->argument->visit(this);
    auto value = expression->value->visit(this);
    //  TODO:  user-defined method
//    auto method = target->getAttribute("setitem");
//    return callOperator(method, {argument, value});
    return nullptr;
}

ObjPtr Interpreter::visit(Variable *expression) {
    return getVariable(expression->name, expression->level);
}

ObjPtr Interpreter::visit(CallExpression *expression) {
    ObjPtr object = expression->target->visit(this);

    auto callable = getCallable(std::move(object));
    checkArguments(callable, expression->argsList.size());

    auto arguments = evaluateArguments(expression->argsList);

    return call(callable, arguments);
}

ObjPtr Interpreter::visit(SuperClass *expression) {
    auto super = getVariable("super", expression->level);
    auto result = super->getAttribute(expression->token.body);
    auto method = std::dynamic_pointer_cast<Callable>(result);
    if (method) {
        auto instance = getVariable("this", expression->level);
        return ObjPtr(new ClassMethod(method, std::dynamic_pointer_cast<Instance>(instance)));
    }
    return result;
}

ObjPtr Interpreter::visit(GetItem *expression) {
    auto target = expression->target->visit(this);
    auto argument = expression->argument->visit(this);
    //  TODO:  user-defined method
//    auto method = target->getAttribute("getitem");
//    return callOperator(method, {argument});
    return nullptr;
}

ObjPtr Interpreter::visit(GetAttribute *expression) {
    auto target = expression->target->visit(this);
    return target->getAttribute(expression->name);
}