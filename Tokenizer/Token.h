#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <string>
#include <array>
#include <vector>

struct Token {
    enum tokenType {
        NUMBER, BOOL, NONE,
        ADD, SUB, MUL, DIV,
        EQUAL, GREATER, GREATER_OR_EQUAL, LESS, LESS_OR_EQUAL, NOT_EQUAL,
        IDENTIFIER, ASSIGNMENT,
        DELIMITER, SEPARATOR,
        BRACKET_OPEN, BRACKET_CLOSE, BRACKET,
        BLOCK_OPEN, BLOCK_CLOSE,
        IF, ELSE, WHILE,
        FUNCTION,
        BREAK, CONTINUE, RETURN,
        PROGRAM_END, ERROR
    };

    tokenType type;
    std::string body;

    Token(tokenType type, const std::string &body);
};

#endif //INTERPRETER_TOKEN_H
