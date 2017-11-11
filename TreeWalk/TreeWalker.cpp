#include "TreeWalker.h"
#include "../Parser/Statement/Statement.h"

void TreeWalker::visitStatements(std::vector<Statement *> &statements) {
    for (auto statement : statements)
        statement->visit(this);
}