#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <string>
#include <stack>
#include "../Object/Object.h"
#include "../Tokenizer/Token.h"

class Binary;
class Unary;
class Literal;
class SetVariable;
class Variable;

class Statement;
class ExpressionStatement;
class IfStatement;
class Block;

class Interpreter {
    Object globalScope;
    std::stack<Object *> garbage;

    void evaluateStatements(std::vector<Statement *> &statements);
public:
    void interpret(std::string text);

    Object *evaluate(Binary *expression);
    Object *evaluate(Unary *expression);
    Object *evaluate(Literal *expression);
    Object *evaluate(SetVariable *expression);
    Object *evaluate(Variable *expression);

    void evaluate(ExpressionStatement *statement);
    void evaluate(IfStatement *statement);
    void evaluate(Block *block);
};

#endif //INTERPRETER_INTERPRETER_H
