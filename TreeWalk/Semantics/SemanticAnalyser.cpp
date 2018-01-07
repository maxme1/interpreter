#include "SemanticAnalyser.h"
#include "../../Parser/Statement/Statement.h"

SemanticAnalyser::SemanticAnalyser(std::vector<Statement::ptr> statements) {
    scopes = std::vector<std::map<std::string, bool>>();
    enterScope();
    visitStatements(statements);
}

SemanticAnalyser::~SemanticAnalyser() {
    leaveScope();
}

void SemanticAnalyser::enterScope() {
    scopes.emplace_back();
}

void SemanticAnalyser::leaveScope() {
    assert(!scopes.empty());
    scopes.pop_back();
}

void SemanticAnalyser::setVariable(const std::string &name, bool value) {
    assert(!scopes.empty());
    scopes.back()[name] = value;
}