#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <string>
#include <utility>

struct Token {
    enum tokenType {
        NUMBER, BOOL, NONE, STRING,
        ADD, SUB, MUL, DIV,
        EQUAL, GREATER, GREATER_OR_EQUAL, LESS, LESS_OR_EQUAL, NOT_EQUAL,
        AND, OR, NOT,
        IDENTIFIER, ASSIGNMENT, ATTRIBUTE,
        DELIMITER, SEPARATOR,

        BRACKET_OPEN, BRACKET_CLOSE,
        ITEM_OPEN, ITEM_CLOSE,
        BLOCK_OPEN, BLOCK_CLOSE,

        IF, ELSE, WHILE, FOR, IN, TRY, CATCH, IMPORT,
        FUNCTION, CLASS, EXTENDS,
        BREAK, CONTINUE, RETURN, RAISE,

        ERROR
    };

    tokenType type;
    std::string body;
    int position;

    Token(tokenType type, std::string body, int position) :
            type(type), body(std::move(body)), position(position) {}
};

#endif //INTERPRETER_TOKEN_H
