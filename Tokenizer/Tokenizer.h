#ifndef INTERPRETER_TOKENIZER_H
#define INTERPRETER_TOKENIZER_H

#include <stack>
#include "Token.h"

class Tokenizer {
    std::string text;
    std::string::iterator position;

    Token nextToken();

public:
    Tokenizer(std::string text);

    bool error = false;

    std::vector<Token> tokenize();
};

#endif //INTERPRETER_TOKENIZER_H
