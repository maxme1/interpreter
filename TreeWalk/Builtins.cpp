#include "Interpreter/Interpreter.h"
#include "../Object/Types/String.h"
#include "../Object/Types/Int.h"
#include "../Object/Types/Array.h"
#include "../Object/Native/Native.h"

void Interpreter::populate() {
    defineVariable("Int", Int::build());
    defineVariable("String", String::build());
    defineVariable("Bool", Bool::build());
    defineVariable("Array", Array::build());

//    TODO: populate the rest of the exceptions
    defineVariable("Exception", Exception::build());
    defineVariable("AttributeError", AttributeError::build());

    addFunction("print", $lambda {
        bool first = true;
        for (auto &&arg : positional) {
            if (not first)
                std::cout << " ";
            else
                first = false;
            std::cout << String::toString(arg, interpreter);
        }
        std::cout << std::endl;
        return nullptr;
    }, 1, true);
//    addFunction("input", $lambda {
//        std::string input;
//        std::cin >> input;
//        return New(String(input));
//    });
}

void Interpreter::addFunction(const std::string &name, nativeFunction function, int argumentsCount, bool variable) {
    auto arguments = std::vector<FunctionPrototype::Argument>();
    for (int i = 0; i < argumentsCount; ++i)
        arguments.emplace_back("arg" + std::to_string(i), nullptr, true, variable and i == argumentsCount - 1);
    defineVariable(name, New(NativeFunction(function, arguments)));
}
