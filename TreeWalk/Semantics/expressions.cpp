#include <iostream>
#include <set>
#include "SemanticAnalyser.h"
#include "../../Parser/Parser.h"

ObjPtr SemanticAnalyser::visit(Variable *expression) {
    assert(expression->level == -1);
    if (expression->name == "this") {
        long level = 1;
        for (auto type = types.rbegin(); type != types.rend(); type++) {
            if (*type == BlockType::Method) {
                expression->level = level;
                return nullptr;
            }
            level++;
        }
        throw SyntaxError("No method to bind 'this' to.");
    }

    expression->level = findVariableLevel(expression->name);
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(SetVariable *expression) {
    if (expression->name == "this" or expression->name == "super")
        throw SyntaxError("\"" + expression->name + "\" is not assignable");

    assert(expression->level == -1);
    expression->level = findVariableLevel(expression->name);
    expression->value->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(CallExpression *expression) {
    expression->target->visit(this);
    for (auto &&argument : expression->argsList)
        argument->visit(this);
    auto uniques = std::set<std::string>();
    for (auto &&kwarg : expression->kwargs) {
        if (uniques.count(kwarg->name) > 0)
            throw SyntaxError("Duplicate argument " + kwarg->name);
        kwarg->value->visit(this);
        uniques.insert(kwarg->name);
    }
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(SuperClass *expression) {
    assert(expression->level == -1);
    long level = 1;
    for (auto type = types.rbegin(); type != types.rend(); type++) {
        if (*type == BlockType::Method) {
            expression->level = level;
            return nullptr;
        }
        level++;
    }
    throw SyntaxError("No method to bind 'super'");
}

ObjPtr SemanticAnalyser::visit(Binary *expression) {
    expression->left->visit(this);
    expression->right->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(Unary *expression) {
    expression->argument->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(Literal *expression) {
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(SetAttribute *expression) {
    expression->target->target->visit(this);
    expression->value->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(SetItem *expression) {
    expression->target->target->visit(this);
    expression->target->argument->visit(this);
    expression->value->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(GetItem *expression) {
    expression->target->visit(this);
    expression->argument->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(GetAttribute *expression) {
    expression->target->visit(this);
    return nullptr;
}