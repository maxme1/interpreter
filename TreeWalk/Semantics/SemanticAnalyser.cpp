#include "SemanticAnalyser.h"
#include "../../Parser/Statement/Statement.h"

SemanticAnalysis::SemanticAnalysis(std::vector<Statement *> statements) {
    scopes = std::vector<std::map<std::string, bool>>();
    enterScope();
    visitStatements(statements);
}

SemanticAnalysis::~SemanticAnalysis() {
    leaveScope();
}

void SemanticAnalysis::enterScope() {
    scopes.emplace_back();
}

void SemanticAnalysis::leaveScope() {
    assert(!scopes.empty());
    scopes.pop_back();
}

void SemanticAnalysis::setVariable(const std::string &name, bool value) {
    assert(!scopes.empty());
    scopes.back()[name] = value;
}