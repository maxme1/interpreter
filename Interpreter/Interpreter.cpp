#include <iostream>
#include "Interpreter.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/Parser.h"
#include "../Object/Types/Int.h"
#include "../Object/native.h"
#include "../Object/Types/Function.h"

Interpreter::Interpreter() {
    addScope();
    setVariable("print", new Print());
}

void Interpreter::interpret(std::string text) {
    Tokenizer t = Tokenizer(text);
    auto tokens = t.tokenize();
    if (t.error) {
        //    TODO: it would be better to know the position in the text on error
        std::cout << "Undefined token";
        return;
    }

    Parser p = Parser(tokens);
    auto statements = p.build();
    if (p.error) {
        return;
    }
//    std::cout << statements[0]->str();
    try {
        evaluateStatements(statements);
    } catch (const char *exception) {
        std::cout << exception;
    } catch (Token::tokenType type) {
        std::cout << "Control flow outside loop";
    }
    collect();
}

void Interpreter::addScope() {
    scopes.push_back(new Object());
}

void Interpreter::deleteScope() {
    delete scopes.back();
    scopes.pop_back();
}

Object *Interpreter::getVariable(const std::string &name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); it++) {
        auto result = (*it)->attributes.find(name);
        if (result != (*it)->attributes.end())
            return result->second;
    }
    throw "No variable named " + name;
}

void Interpreter::setVariable(const std::string &name, Object *value) {
    scopes.back()->setAttribute(name, value);
}

//TODO: probably too sugary
void Interpreter::track(std::initializer_list<Object *> objects) {
    for (auto &&object : objects) {
        if (object->zombie())
            garbage.push(object);
    }
}

void Interpreter::collect() {
    if (!garbage.empty()) {
//        std::cout << "\ngarbage: ";
        while (!garbage.empty()) {
            auto obj = garbage.top();
            garbage.pop();
            if (obj and obj->zombie()) {
//                std::cout << obj->str() << " ";
                delete obj;
            }
        }
//        std::cout << "\n";
    }
}

void Interpreter::evaluateStatements(std::vector<Statement *> &statements) {
    for (auto statement : statements) {
        statement->evaluate(this);
        collect();
    }
}

Object *Interpreter::evaluate(Binary *expression) {
    Object *left = expression->left->evaluate(this), *right = expression->right->evaluate(this);
    track({left, right});
    if (expression->type == Token::ADD)
        return left->add(right);
    if (expression->type == Token::SUB)
        return left->sub(right);
    if (expression->type == Token::MUL)
        return left->mul(right);
    if (expression->type == Token::DIV)
        return left->div(right);
    return nullptr;
}

Object *Interpreter::evaluate(Unary *expression) {
    Object *argument = expression->argument->evaluate(this);
    track({argument});
    if (expression->type == Token::ADD)
        return argument->uadd();
    if (expression->type == Token::SUB)
        return argument->usub();
    if (expression->type == Token::BRACKET)
        return argument;
    return nullptr;
}

Object *Interpreter::evaluate(Literal *expression) {
    auto body = expression->body;
    auto type = expression->type;
    if (type == Token::NUMBER)
        return new Int(std::atoi(body.c_str()));
    if (type == Token::BOOL)
        return new Bool(body == "True");
    return nullptr;
}

Object *Interpreter::evaluate(SetVariable *expression) {
    auto value = expression->value->evaluate(this);
    track({value});
    setVariable(expression->name, value);
    return value;
}

Object *Interpreter::evaluate(Variable *expression) {
    return getVariable(expression->body);
}

Object *Interpreter::evaluate(FunctionExpression *expression) {
    Object *obj = expression->target->evaluate(this);
    track({obj});
    if (expression->argsList.size() != obj->functionArguments.size())
        throw "Number of arguments doesn't match";

    addScope();
    for (int i = 0; i < obj->functionArguments.size(); i++) {
        auto arg = expression->argsList[i]->evaluate(this);
        setVariable(obj->functionArguments[i], arg);
    }
    Object *returnObject = nullptr;
    try {
        if (obj->functionBody)
            obj->functionBody->evaluate(this);
        else
            returnObject = obj->__call__(scopes.back());
    } catch (Object *object) {
        returnObject = object;
    } catch (Token::tokenType type) {
//        TODO: move to syntactic errors
//        TODO: now the function still return none on this exception
        std::cout << "Control flow outside loop";
    } catch (const char *other) {
        deleteScope();
        throw other;
    }
    if (!returnObject)
        returnObject = new None;
    deleteScope();
    return returnObject;
}

void Interpreter::evaluate(ReturnStatement *statement) {
    if (statement->expression)
        throw statement->expression->evaluate(this);
    throw new None;
}

void Interpreter::evaluate(ExpressionStatement *statement) {
//    TODO: probably can collect all here
    auto obj = statement->expression->evaluate(this);
    track({obj});
//    std::cout << obj->str() << "\n";
}

void Interpreter::evaluate(IfStatement *statement) {
    auto cond = statement->condition->evaluate(this);
    track({cond});
    if (cond->asBool()) {
        if (statement->left)
            statement->left->evaluate(this);
    } else if (statement->right)
        statement->right->evaluate(this);
}

void Interpreter::evaluate(WhileStatement *statement) {
    auto cond = statement->condition->evaluate(this);
    track({cond});
    while (cond->asBool()) {
        try {
            if (statement->body)
                statement->body->evaluate(this);
        } catch (Token::tokenType type) {
            if (type == Token::BREAK)
                break;
        }
        cond = statement->condition->evaluate(this);
        track({cond});
    }
}

void Interpreter::evaluate(ControlFlow *statement) {
    throw statement->type;
}

void Interpreter::evaluate(Block *block) {
    evaluateStatements(block->statements);
}

void Interpreter::evaluate(FunctionDefinition *statement) {
    setVariable(statement->name, new Function(statement->arguments, statement->body));
}