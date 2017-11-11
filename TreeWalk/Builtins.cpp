#include "Interpreter/Interpreter.h"
#include "../Object/Types/String.h"
#include "../Object/Types/Int.h"
#include "../Object/Types/Array.h"
#include "../Object/Native/Native.h"

ObjPtr print(ArgsList args, Interpreter * interpreter){
    bool first = true;
    for (auto &&arg : args) {
        if (not first) {
            std::cout << " ";
        } else
            first = false;
        std::cout << arg->asString();
    }
    std::cout << std::endl;
    return nullptr;
}

void Interpreter::populate() {
//    setVariable("Int", Int::build());
//    setVariable("String", String::build());
//    setVariable("Bool", Bool::build());
//    setVariable("Array", Array::build());

//    TODO: populate the rest of the exceptions
//    setVariable("Exception", Exception::build());
//    setVariable("AttributeError", AttributeError::build());

    addFunction("print", print, 1);

//    addFunction("print", $lambda {
//        bool first = true;
//        for (auto &&arg : args) {
//            if (not first) {
//                std::cout << " ";
//            } else
//                first = false;
//            std::cout << arg->asString();
//        }
//        std::cout << std::endl;
//        return nullptr;
//    }, 1);
//    addFunction("input", $lambda {
//        std::string input;
//        std::cin >> input;
//        return New(String(input));
//    });
}

void Interpreter::addFunction(const std::string &name, nativeFunction function, int argumentsCount, bool unlimited) {
    setVariable(name, New(NativeFunction(function, argumentsCount, unlimited)));
}
