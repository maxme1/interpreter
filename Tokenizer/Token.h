#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <string>
#include <array>
#include <vector>

struct Token {
    enum tokenType {
        NUMBER, BOOL, NONE, STRING,
        ADD, SUB, MUL, DIV,
        EQUAL, GREATER, GREATER_OR_EQUAL, LESS, LESS_OR_EQUAL, NOT_EQUAL,
        IDENTIFIER, ASSIGNMENT, ATTRIBUTE,
        DELIMITER, SEPARATOR,
        BRACKET_OPEN, BRACKET_CLOSE, BRACKET,
        ITEM_OPEN, ITEM_CLOSE,
        BLOCK_OPEN, BLOCK_CLOSE,
        IF, ELSE, WHILE,
        FUNCTION, CLASS,
        BREAK, CONTINUE, RETURN,
        PROGRAM_END, ERROR, COMMENT
    };

    tokenType type;
    std::string body;

    Token(tokenType type, const std::string &body);
};

#endif //INTERPRETER_TOKEN_H
