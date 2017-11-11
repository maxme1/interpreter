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
    ObjPtr left = expression->left->visit(this), right = expression->right->visit(this);
    auto name = binary.find(expression->token.type);
    if (name != binary.end()) {
        auto method = left->findAttribute(name->second);
        if (method)
            return callOperator(method, {right});
    }

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
//    throw Wrap(new Exception("Operator not defined"));
}

ObjPtr Interpreter::visit(Literal *expression) {
    if (expression->ofType(Token::NUMBER))
        return New(Int(std::stoi(expression->token.body)));
//    if (expression->ofType(Token::BOOL))
//        return New(Bool(expression->token.body == "True"));
    if (expression->ofType(Token::NONE))
        return New(None());
//    if (expression->ofType(Token::STRING)) {
//        auto body = expression->token.body;
//        return New(String(body.substr(1, body.size() - 2)));
//    }
    return nullptr;
}

ObjPtr Interpreter::visit(SetVariable *expression) {
    auto value = expression->value->visit(this);
    setVariable(expression->name, value);
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
    //    user-defined method
    auto method = target->getAttribute("setitem");
    return callOperator(method, {argument, value});
}

ObjPtr Interpreter::visit(Variable *expression) {
    return getVariable(expression->name);
}

ObjPtr Interpreter::visit(FunctionExpression *expression) {
    ObjPtr object = expression->target->visit(this);

//    auto classObject = std::dynamic_pointer_cast<Class>(object);
//    if (classObject) {
//        auto instance = classObject->makeInstance(nullptr);
//        ObjPtr init;
//        addScope(classObject->context);
//        try {
//            init = instance->findAttribute("init");
//            if (init)
//                callFunction(init, expression->argsList);
//        } catch (ExceptionWrapper &e) {
//            deleteScope();
//            throw;
//        }
//        deleteScope();
//        if (!init and !expression->argsList.empty())
//            throw Wrap(new Exception("Default constructor does not receive arguments"));
//        return instance;
//    }

    return callFunction(object, expression->argsList);
}

ObjPtr Interpreter::visit(GetItem *expression) {
    auto target = expression->target->visit(this);
    auto argument = expression->argument->visit(this);
    //    user-defined method
    auto method = target->getAttribute("getitem");
    return callOperator(method, {argument});
}

ObjPtr Interpreter::visit(GetAttribute *expression) {
    auto target = expression->target->visit(this);
    return target->getAttribute(expression->name);
}