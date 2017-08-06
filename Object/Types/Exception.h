#ifndef INTERPRETER_EXCEPTION_H
#define INTERPRETER_EXCEPTION_H

#include "../Object.h"

class Exception : public Object {
    std::string body;

public:
    explicit Exception(const std::string &body) : body(body) {}

    std::string asString() override {
        return "Exception: " + body;
    }
};

#endif //INTERPRETER_EXCEPTION_H
