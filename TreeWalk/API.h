//#ifndef INTERPRETER_API_H
//#define INTERPRETER_API_H
//
//#include "../Object/Object.h"
//#include "Interpreter/Interpreter.h"
//
//class API {
//    friend class Function;
//public:
//    Interpreter *interpreter;
//    explicit API(Interpreter *interpreter) : interpreter(interpreter) {}
//
//    ObjPtr getVariable(const std::string &name) {
//        return interpreter->getVariable(name);
//    }
//
//    void setVariable(const std::string &name, ObjPtr value) {
//        interpreter->setVariable(name, value);
//    }
//
//    ObjPtr call(ObjPtr object, const std::vector<ObjPtr > &arguments) {
//        return interpreter->callOperator(object, arguments);
//    }
//
//    typedef Interpreter::ExceptionWrapper Wrap;
//};
//
//typedef API::Wrap Wrap;
//
//#endif //INTERPRETER_API_H
