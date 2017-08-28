#ifndef INTERPRETER_NONE_H
#define INTERPRETER_NONE_H


#include "../Native/Native.h"

//TODO: None should be a singleton
$class(None)
    None() = default;

    bool asBool() override { return false; }

    std::string asString() override { return "None"; }
};


#endif //INTERPRETER_NONE_H
