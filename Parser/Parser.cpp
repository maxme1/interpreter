#include "Parser.h"

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {
    position = this->tokens.begin();
}

bool Parser::matches(std::initializer_list<Token::tokenType> types) {
    if (position == tokens.end())
        throw ProgramEnd();

    for (auto type : types)
        if (type == position->type)
            return true;
    return false;
}

Token Parser::advance() {
    auto current = *position;
    position++;
    return current;
}

Token Parser::require(std::initializer_list<Token::tokenType> types) {
    if (!matches(types))
        throw Token(*position);

    return advance();
}
