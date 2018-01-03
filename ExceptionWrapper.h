#ifndef INTERPRETER_EXCEPTIONWRAPPER_H
#define INTERPRETER_EXCEPTIONWRAPPER_H

#include <vector>
#include "Tokenizer/Token.h"

class BaseExceptionWrapper {

public:
    std::vector<Token> traceback{};
//    TODO: maybe add method getMessage
    std::string message{};
    BaseExceptionWrapper() = default;

    explicit BaseExceptionWrapper(const std::string &message) : message(message) {}

    void push(Token token) {
        traceback.push_back(token);
    }
};

#endif //INTERPRETER_EXCEPTIONWRAPPER_H
