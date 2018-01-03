#ifndef INTERPRETER_TOKENIZER_H
#define INTERPRETER_TOKENIZER_H

#include <vector>
#include "Token.h"
#include "../ExceptionWrapper.h"

class Tokenizer {
    struct SyntaxError : public BaseExceptionWrapper {
        explicit SyntaxError(Token token) {
            message = "Unrecognized token " + token.body;
            push(token);
        }
    };

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
