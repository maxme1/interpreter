#include <iostream>
#include "Interpreter.h"
#include "../../Parser/Parser.h"
#include "../../Object/Native/Native.h"
#include "../../Object/Exception.h"

void Interpreter::visit(ReturnStatement *statement) {
    if (statement->expression)
        throw ReturnException(statement->expression->visit(this));
    throw ReturnException();
}

void Interpreter::visit(RaiseStatement *statement) {
//    throw Wrap(statement->expression->visit(this));
}

void Interpreter::visit(ImportStatement *statement) {
    auto path = statement->path;
//    TODO: for now the imported file must be in the same directory with the executable
    Interpreter source = Interpreter();
//    if (!source.interpretFile(path))
//        throw Wrap(new ImportError("Could not import " + path));
    setVariable(path, source.getContext());
}

void Interpreter::visit(ExpressionStatement *statement) {
    statement->expression->visit(this);
}

void Interpreter::visit(IfStatement *statement) {
    auto cond = statement->condition->visit(this);
    if (cond->asBool()) {
        if (statement->left)
            statement->left->visit(this);
    } else if (statement->right)
        statement->right->visit(this);
}

void Interpreter::visit(TryStatement *statement) {
//    try {
//        statement->block->visit(this);
//    } catch (ExceptionWrapper &e) {
////        TODO: too many cases:
//        auto theClass = std::dynamic_pointer_cast<Class>(e.exception);
//        if (!theClass) {
//            auto instance = std::dynamic_pointer_cast<Instance>(e.exception);
//            if (instance)
//                theClass = instance->getClass();
//        }
//        assert(theClass);
//
//        for (auto &&item : statement->catches) {
//            auto variants = evaluateArguments(item->arguments);
//            for (auto &&variant : variants) {
////                TODO: throw here
//                auto arg = std::dynamic_pointer_cast<Class>(variant);
//                if (arg)
//                    if (isDerived(theClass, arg)) {
//                        item->block->visit(this);
//                        return;
//                    }
//            }
//        }
//        if (!statement->catches.empty())
//            throw;
//    }
}

void Interpreter::visit(WhileStatement *statement) {
    auto cond = statement->condition->visit(this);
    while (cond->asBool()) {
        try {
            if (statement->body)
                statement->body->visit(this);
        } catch (BreakException) {
            break;
        } catch (ContinueException) {}
        cond = statement->condition->visit(this);
    }
}

void Interpreter::visit(ControlFlow *statement) {
    if (statement->type == Token::CONTINUE)
        throw ContinueException();
    throw BreakException();
}

void Interpreter::visit(Block *block) {
    visitStatements(block->statements);
}

void Interpreter::visit(FunctionDefinition *statement) {
    setVariable(statement->name,
                New(Function(statement->arguments, statement->body, statement->unlimited, getContext())));
}

void Interpreter::visit(ClassDefinition *statement) {
//    Class::ptr superclass;
//    if (statement->superclass) {
//        auto temp = statement->superclass->visit(this);
//        superclass = std::dynamic_pointer_cast<Class>(temp);
//        if (!superclass)
//            throw Wrap(new ValueError("Cannot subclass instances"));
//    }
//    auto context = getContext();
//    addScope();
//    try {
//        statement->body->visit(this);
//    } catch (ReturnException &e) {
////        TODO: move to syntactic errors
//        throw Wrap(new Exception("Return outside function"));
//    } catch (FlowException &e) {
////        TODO: move to syntactic errors
//        throw Wrap(new Exception("Control flow outside loop"));
//    } catch (Wrap &e) {
//        deleteScope();
//        throw;
//    }
//    auto classScope = std::make_shared<Class>(statement->name, getContext(), superclass, context);
//    deleteScope();
//    setVariable(statement->name, classScope);
}