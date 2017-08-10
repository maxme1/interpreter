#ifndef INTERPRETER_TOKENIZER_H
#define INTERPRETER_TOKENIZER_H

#include <vector>
#include "Token.h"

class Tokenizer {
public:
    explicit Tokenizer(std::string text);

    std::vector<Token> tokenize();
    bool error = false;
private:
    std::string text;
    std::string::iterator position;

    Token nextToken();
};

#endif //INTERPRETER_TOKENIZER_H
