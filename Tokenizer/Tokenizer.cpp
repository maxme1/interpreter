#include <iostream>
#include <map>
#include "Tokenizer.h"

#define tk(a) queue.push(Token(a, std::string(begin, position), position - text.begin())); return true;

Tokenizer::Tokenizer(const std::string &text) : text(text) {
    if (this->text.back() != '\n')
        this->text.push_back('\n');
    position = this->text.begin();
}

std::vector<Token> Tokenizer::tokenize() {
    while (getNextToken());
    auto result = std::vector<Token>();
    while (!queue.empty()) {
        auto next = queue.front();
        queue.pop();
        if (!result.empty())
            if (result.back().type == Token::DELIMITER) {
                if (next.type == Token::BLOCK_OPEN)
                    result.pop_back();
            }
        result.push_back(next);
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
//        {'{', Token::BLOCK_OPEN},
//        {'}', Token::BLOCK_CLOSE},
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
        {"extends",  Token::EXTENDS},
};

bool Tokenizer::getNextToken() {
    auto begin = position;
    while (position != text.end() and *position == ' ')
        position++;

//    comment
    if (*position == '#') {
        while (position != text.end() and *position != '\n')
            position++;
        return true;
    }

    if (atBegin) {
//        blank lines
        if (*position == '\n') {
            while (position != text.end() and *position == '\n')
                position++;
            return true;
        }

        atBegin = false;
        int indent = position - begin;
        int diff = currentIndent - indent;
        if (diff % indentLength) {
            error = true;
            return false;
        }
        int amount = abs(diff) / indentLength;
        for (int i = 0; i < amount; ++i)
            if (diff > 0)
                queue.push(Token(Token::BLOCK_CLOSE, "}", position - text.begin()));
            else
                queue.push(Token(Token::BLOCK_OPEN, "{", position - text.begin()));
        currentIndent = indent;
        return true;
    }
    begin = position;

    if (position == text.end())
        return false;

//    string
    if (*position == '\'') {
        position++;
        while (position != text.end() and *position != '\'')
            position++;
        if (*position == '\'') {
            position++;
            tk(Token::STRING);
        }
        tk(Token::ERROR);
    }
//    number
    if (isdigit(*position)) {
        while (isdigit(*position))
            position++;
        tk(Token::NUMBER);
    }
//    identifier or reserved
    if (isalpha(*position) or *position == '_') {
        while (isalnum(*position) or *position == '_')
            position++;
        auto result = reserved.find(std::string(begin, position));
        if (result != reserved.end()) {
            tk(result->second);
        }
        tk(Token::IDENTIFIER);
    }
//    two symbols
    if ((position + 1) != text.end()) {
        auto result = two_symbols.find(std::string(begin, position + 2));
        if (result != two_symbols.end()) {
            position += 2;
            tk(result->second);
        }
    }
//    one symbol
    {
        auto result = one_symbol.find(*position);
        if (result != one_symbol.end()) {
            position++;
            tk(result->second);
        }
    }

    if (*position == '\n') {
        position++;
        atBegin = true;
        tk(Token::DELIMITER);
    }

    position++;
    queue.push(Token(Token::ERROR, std::string(begin, position), position - text.begin()));
    error = true;
    return false;
}
