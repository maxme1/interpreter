#ifndef INTERPRETER_EXCEPTIONWRAPPER_H
#define INTERPRETER_EXCEPTIONWRAPPER_H

#include <vector>
#include "Tokenizer/Token.h"

class BaseExceptionWrapper {
    std::vector<Token> traceback;

public:
    void push(Token token) {
        traceback.push_back(token);
    }
};

#endif //INTERPRETER_EXCEPTIONWRAPPER_H
