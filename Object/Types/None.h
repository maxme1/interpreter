#ifndef INTERPRETER_NONE_H
#define INTERPRETER_NONE_H


#include "../Object.h"

class None : public Object {
public:
    bool asBool() override { return false; }

    std::string asString() override { return "None"; }
};


#endif //INTERPRETER_NONE_H
