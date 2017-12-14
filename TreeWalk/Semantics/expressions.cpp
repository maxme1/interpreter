#include <iostream>
#include "SemanticAnalyser.h"
#include "../../Parser/Parser.h"

ObjPtr SemanticAnalyser::visit(Variable *expression) {
    assert(expression->level == -1);

//    TODO: too ugly
    if (expression->name == "this") {
        bool found = false, inClass = false;
        for (auto type = types.rbegin(); type != types.rend(); type++) {
            if (*type == BlockType::Function)
                found = true;
            if (*type == BlockType::Class)
                inClass = true;
            if (found and inClass)
                break;
        }
        assert(found and inClass);
        expression->level = 1;
        return nullptr;
    }

    for (int i = 0; i < scopes.size(); ++i) {
        auto scope = scopes[scopes.size() - i - 1];
        auto find = scope.find(expression->name);
        if (find != scope.end())
            if (find->second) {
//            TODO: setter?
                expression->level = i;
                return nullptr;
            } else {
//                TODO: throw
                assert(false);
            }
    }
    expression->level = scopes.size() - 1;
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(SetVariable *expression) {
    assert(expression->name != "this");
//    TODO: combine
    assert(expression->level == -1);
    for (int i = 0; i < scopes.size(); ++i) {
        auto scope = scopes[scopes.size() - i - 1];
        auto find = scope.find(expression->name);
        if (find != scope.end())
            if (find->second) {
//            TODO: setter?
                expression->level = i;
                break;
            } else {
//                TODO: throw
                assert(false);
            }
    }
    expression->level = scopes.size() - 1;
    expression->value->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(CallExpression *expression) {
    expression->target->visit(this);
    for (auto &&argument : expression->argsList) {
        argument->visit(this);
    }
    return nullptr;
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
//    TODO: also looks bad
    expression->target->target->visit(this);
    expression->value->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalyser::visit(SetItem *expression) {
//    TODO: also looks bad
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