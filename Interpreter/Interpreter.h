#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <string>
#include "../Object/Object.h"
#include "../Tokenizer/Token.h"

class Binary;
class Unary;
class NumberToken;
class SetVariable;
class Variable;

class ExpressionStatement;

class Interpreter {
    Object globalScope;
public:
    void interpret(std::string text);

    Object *evaluate(Binary *expression);
    Object *evaluate(Unary *expression);
    Object *evaluate(NumberToken *expression);
    Object *evaluate(SetVariable *expression);
    Object *evaluate(Variable *expression);

    void evaluate(ExpressionStatement *expression);
};

#endif //INTERPRETER_INTERPRETER_H
