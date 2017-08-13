#ifndef INTERPRETER_TOKENIZER_H
#define INTERPRETER_TOKENIZER_H

#include <vector>
#include <queue>
#include "Token.h"

class Tokenizer {
public:
    explicit Tokenizer(const std::string &text);

    std::vector<Token> tokenize();
    std::queue<Token> queue;
    bool error = false;
    bool atBegin = true;
    int currentIndent = 0;
    int indentLength = 4;
private:
    std::string text;
    std::string::iterator position;

    bool getNextToken();
};

#endif //INTERPRETER_TOKENIZER_H
