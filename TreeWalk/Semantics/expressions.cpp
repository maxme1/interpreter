#include <iostream>
#include "SemanticAnalyser.h"
#include "../../Parser/Parser.h"

ObjPtr SemanticAnalysis::visit(Variable *expression) {
    assert(expression->level == -1);
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

ObjPtr SemanticAnalysis::visit(SetVariable *expression) {
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

ObjPtr SemanticAnalysis::visit(FunctionExpression *expression) {
    expression->target->visit(this);
    for (auto &&argument : expression->argsList) {
        argument->visit(this);
    }
    return nullptr;
}

ObjPtr SemanticAnalysis::visit(Binary *expression) {
    expression->left->visit(this);
    expression->right->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalysis::visit(Unary *expression) {
    expression->argument->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalysis::visit(Literal *expression) {
    return nullptr;
}

ObjPtr SemanticAnalysis::visit(SetAttribute *expression) {
//    TODO: also looks bad
    expression->target->target->visit(this);
    expression->value->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalysis::visit(SetItem *expression) {
//    TODO: also looks bad
    expression->target->target->visit(this);
    expression->target->argument->visit(this);
    expression->value->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalysis::visit(GetItem *expression) {
    expression->target->visit(this);
    expression->argument->visit(this);
    return nullptr;
}

ObjPtr SemanticAnalysis::visit(GetAttribute *expression) {
    expression->target->visit(this);
    return nullptr;
}