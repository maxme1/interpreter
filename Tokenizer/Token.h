#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <string>
#include <array>
#include <vector>

struct Token {
    enum tokenType {
        NUMBER,
        ADD, SUB, MUL, DIV,
        IDENTIFIER, EQUALS,
        DELIMITER,
        BRACKET_OPEN, BRACKET_CLOSE, BRACKET,
        PROGRAM_END, ERROR
    };

    tokenType type;
    std::string body;

    Token(tokenType type, const std::string &body);
};

#endif //INTERPRETER_TOKEN_H
