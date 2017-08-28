#include "Interpreter.h"
#include "../Object/Types/String.h"
#include "../Object/Types/Int.h"
#include "../Object/Types/Array.h"
#include "../Object/Types/Range.h"

void Interpreter::populate() {
    setVariable("Int", Int::build());
    setVariable("String", String::build());
    setVariable("Bool", Bool::build());
    setVariable("Array", Array::build());

    setVariable("range", Range::build());

//    TODO: populate the rest of the exceptions
    setVariable("Exception", Exception::build());
    setVariable("AttributeError", AttributeError::build());

    addFunction("print", $lambda {
        bool first = true;
        for (auto &&arg : args) {
            if (not first) {
                std::cout << " ";
            } else
                first = false;
            std::cout << String::toString({arg}, api);
        }
        std::cout << std::endl;
        return nullptr;
    }, 0, true);
    addFunction("input", $lambda {
        std::string input;
        std::cin >> input;
        return New(String(input));
    });
}

void Interpreter::addFunction(const std::string &name, nativeFunction function, int argumentsCount, bool unlimited) {
    setVariable(name, New(NativeFunction(function, argumentsCount, unlimited)));
}
