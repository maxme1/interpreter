#include "Parser.h"

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {
    position = this->tokens.begin();
}

bool Parser::matches(std::initializer_list<Token::tokenType> types, int shift) {
    auto pos = position + shift;
    if (pos == tokens.end())
        throw SyntaxError("Unexpected end of file");

    for (auto type : types)
        if (type == pos->type)
            return true;
    return false;
}

Token Parser::advance() {
    auto current = *position;
    position++;
    return current;
}

Token Parser::require(std::initializer_list<Token::tokenType> types) {
    if (!matches(types)) {
        auto err = SyntaxError("Unexpected token \"" + position->body + "\"");
        err.push(*position);
        throw err;
    }
    return advance();
}
