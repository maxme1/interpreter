#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <string>
#include <array>
#include <vector>

struct Token {
    enum tokenType {
        NUMBER, BOOL,
        ADD, SUB, MUL, DIV,
        IDENTIFIER, ASSIGNMENT,
        DELIMITER,
        BRACKET_OPEN, BRACKET_CLOSE, BRACKET,
        BLOCK_OPEN, BLOCK_CLOSE,
        PROGRAM_END, ERROR
    };

    tokenType type;
    std::string body;

    Token(tokenType type, const std::string &body);
};

#endif //INTERPRETER_TOKEN_H
