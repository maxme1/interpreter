#include <iostream>
#include <map>
#include "Tokenizer.h"

#define tk(a) Token(a, std::string(begin, position))

Tokenizer::Tokenizer(std::string text) : text(text) {
    position = this->text.begin();
}

std::vector<Token> Tokenizer::tokenize() {
    auto result = std::vector<Token>();
    auto next = nextToken();
    while (next.type != Token::PROGRAM_END) {
        result.push_back(next);
        if (next.type == Token::ERROR) {
            error = true;
            return result;
        }
        next = nextToken();
    }
    return result;
}

std::map<char, Token::tokenType> one_symbol = {
        {'-', Token::SUB},
        {'+', Token::ADD},
        {'*', Token::MUL},
        {'/', Token::DIV},
        {'=', Token::ASSIGNMENT},
        {';', Token::DELIMITER},
        {',', Token::SEPARATOR},
        {'(', Token::BRACKET_OPEN},
        {')', Token::BRACKET_CLOSE},
        {'{', Token::BLOCK_OPEN},
        {'}', Token::BLOCK_CLOSE},
        {'>', Token::GREATER},
        {'<', Token::LESS},
};

std::map<std::string, Token::tokenType> two_symbols = {
        {"==", Token::EQUAL},
        {"<=", Token::LESS_OR_EQUAL},
        {">=", Token::GREATER_OR_EQUAL},
        {"!=", Token::NOT_EQUAL},
};

std::map<std::string, Token::tokenType> reserved = {
        {"True",     Token::BOOL},
        {"False",    Token::BOOL},
        {"if",       Token::IF},
        {"else",     Token::ELSE},
        {"while",    Token::WHILE},
        {"break",    Token::BREAK},
        {"continue", Token::CONTINUE},
        {"return",   Token::RETURN},
        {"def",      Token::FUNCTION},
};

Token Tokenizer::nextToken() {
    while (position != text.end() and (*position == ' ' or *position == '\n' or *position == '\r'))
        position++;

    if (position == text.end())
        return Token(Token::PROGRAM_END, "<<<");

    auto begin = position;
//    number
    if (isdigit(*position)) {
        while (isdigit(*position))
            position++;
        return tk(Token::NUMBER);
    }
//    identifier or reserved
    if (isalpha(*position) or *position == '_') {
        while (isalnum(*position) or *position == '_')
            position++;
        auto result = std::string(begin, position);
        if (reserved.find(result) != reserved.end()) {
            return tk(reserved[result]);
        }
        return tk(Token::IDENTIFIER);
    }
//    two symbols
    if ((position + 1) != text.end()) {
        auto two = std::string(begin, position + 2);
        if (two_symbols.find(two) != two_symbols.end()) {
            position += 2;
            return tk(two_symbols[two]);
        }
    }
//    one symbol
    if (one_symbol.find(*position) != one_symbol.end()) {
        position++;
        return tk(one_symbol[*begin]);
    }

    return tk(Token::ERROR);
}
