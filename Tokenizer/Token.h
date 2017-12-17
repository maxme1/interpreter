#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <string>
#include <utility>

struct Token {
    enum tokenType {
        NUMBER, BOOL, NONE, STRING,
        ADD, SUB, MUL, DIV,
        EQUAL, GREATER, GREATER_OR_EQUAL, LESS, LESS_OR_EQUAL, NOT_EQUAL,
        IDENTIFIER, ASSIGNMENT, ATTRIBUTE,
        DELIMITER, SEPARATOR,

        BRACKET_OPEN, BRACKET_CLOSE,
        ITEM_OPEN, ITEM_CLOSE,
        BLOCK_OPEN, BLOCK_CLOSE,

        IF, ELSE, WHILE, TRY, CATCH, IMPORT,
        FUNCTION, CLASS, VAR, SUPER,
        BREAK, CONTINUE, RETURN, RAISE,

        ERROR
    };

    tokenType type;
    std::string body;
    long line{}, column{};

    Token(tokenType type, std::string body) :
            type(type), body(std::move(body)) {}

    void set_position(long line, long column) {
        this->line = line;
        this->column = column;
    }
};

#endif //INTERPRETER_TOKEN_H
