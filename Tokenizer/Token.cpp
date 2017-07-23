#include <iostream>
#include <iomanip>
#include "Token.h"

Token::Token(Token::tokenType type, const std::string &body) : type(type), body(body) {}
