#include <iostream>
#include <map>
#include <utility>
#include "Tokenizer.h"

#define tk(a) Token(a, std::string(begin, position))

Tokenizer::Tokenizer(std::string text) : text(std::move(text)) {
//    if (text.back() != '\n')
//        text.push_back('\n');
    position = this->text.begin();
}

std::vector<Token> Tokenizer::tokenize() {
    auto result = std::vector<Token>();
//    line by line
    int line = 1;
    while (position != text.end()) {
        auto begin = position;

        while (*position != '\n') {
//            blanks
            while (*position == ' ' or *position == '\r')
                position++;
//           comment
            if (*position == '#')
                while (*position != '\n')
                    position++;

            auto column = 1 + std::distance(begin, position);
            auto token = nextToken();
            token.set_position(line, column);
            result.push_back(token);
            if (token.type == Token::ERROR) {
                this->error = true;
                return result;
            }
        }
        position++;
        line++;
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
        {'[', Token::ITEM_OPEN},
        {']', Token::ITEM_CLOSE},
        {'.', Token::ATTRIBUTE},
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
        {"None",     Token::NONE},
        {"if",       Token::IF},
        {"else",     Token::ELSE},
        {"while",    Token::WHILE},
        {"try",      Token::TRY},
        {"catch",    Token::CATCH},
        {"break",    Token::BREAK},
        {"continue", Token::CONTINUE},
        {"return",   Token::RETURN},
        {"raise",    Token::RAISE},
        {"def",      Token::FUNCTION},
        {"class",    Token::CLASS},
        {"import",   Token::IMPORT},
};

Token Tokenizer::nextToken() {
    auto begin = position;
//    string
    if (*position == '\'') {
        position++;
        while (position != text.end() and *position != '\'')
            position++;
        if (*position == '\'') {
            position++;
            return tk(Token::STRING);
        }
        return tk(Token::ERROR);
    }
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
        auto result = reserved.find(std::string(begin, position));
        if (result != reserved.end()) {
            return tk(result->second);
        }
        return tk(Token::IDENTIFIER);
    }
//    two symbols
    if ((position + 1) != text.end()) {
        auto result = two_symbols.find(std::string(begin, position + 2));
        if (result != two_symbols.end()) {
            position += 2;
            return tk(result->second);
        }
    }
//    one symbol
    {
        auto result = one_symbol.find(*position);
        if (result != one_symbol.end()) {
            position++;
            return tk(result->second);
        }
    }

    while (position != text.end() and *position != ' ' and *position != '\r')
        position++;
    return tk(Token::ERROR);
}
