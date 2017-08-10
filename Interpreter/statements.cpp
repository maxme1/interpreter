#include <iostream>
#include "Interpreter.h"
#include "../Parser/Parser.h"
#include "../Object/Native/Native.h"
#include "../Object/Types/Exception.h"

void Interpreter::evaluate(ReturnStatement *statement) {
    if (statement->expression)
        throw ReturnException(track(statement->expression->evaluate(this)));
    throw ReturnException();
}

void Interpreter::evaluate(RaiseStatement *statement) {
    throw Wrap(track(statement->expression->evaluate(this)));
}

void Interpreter::evaluate(ExpressionStatement *statement) {
    track(statement->expression->evaluate(this));
}

void Interpreter::evaluate(IfStatement *statement) {
    auto cond = track(statement->condition->evaluate(this));
    if (cond->asBool()) {
        if (statement->left)
            statement->left->evaluate(this);
    } else if (statement->right)
        statement->right->evaluate(this);
}

void Interpreter::evaluate(TryStatement *statement) {
    try {
        statement->block->evaluate(this);
    } catch (ExceptionWrapper &e) {
//        TODO: too many cases:
        auto theClass = dynamic_cast<Class *>(e.exception);
        if (!theClass) {
            auto instance = dynamic_cast<Instance *>(e.exception);
            if (instance)
                theClass = instance->getClass();
        }
        assert(theClass);

        for (auto &&item : statement->catches) {
            auto variants = evaluateArguments(item->arguments);
            for (auto &&variant : variants) {
//                TODO: throw here
                auto arg = dynamic_cast<Class *>(variant);
                if (arg)
                    if (isDerived(theClass, arg)) {
                        item->block->evaluate(this);
                        return;
                    }
            }
        }
        if (!statement->catches.empty())
            throw;
    }
}

void Interpreter::evaluate(WhileStatement *statement) {
    auto cond = track(statement->condition->evaluate(this));
    while (cond->asBool()) {
        try {
            if (statement->body)
                statement->body->evaluate(this);
        } catch (BreakException) {
            break;
        } catch (ContinueException) {}
        cond = track(statement->condition->evaluate(this));
    }
}

void Interpreter::evaluate(ControlFlow *statement) {
    if (statement->type == Token::CONTINUE)
        throw ContinueException();
    throw BreakException();
}

void Interpreter::evaluate(Block *block) {
    evaluateStatements(block->statements);
}

void Interpreter::evaluate(FunctionDefinition *statement) {
    setVariable(statement->name,
                new Function(statement->arguments, statement->body, statement->unlimited, getContext()));
}

void Interpreter::evaluate(ClassDefinition *statement) {
    Class *superclass = nullptr;
    if (statement->superclass) {
        auto temp = track(statement->superclass->evaluate(this));
        superclass = dynamic_cast<Class *>(temp);
        if (!superclass)
            throw Wrap(new Exception("Cannot subclass instances"));
    }
    auto context = getContext();
    addScope();
    try {
        statement->body->evaluate(this);
    } catch (ReturnException &e) {
//        TODO: move to syntactic errors
        throw Wrap(new Exception("Return outside function"));
    } catch (FlowException &e) {
//        TODO: move to syntactic errors
        throw Wrap(new Exception("Control flow outside loop"));
    } catch (Wrap &e) {
        deleteScope();
        throw;
    }
//    TODO: for now the class does nothing with its context
    auto classScope = track(new Class(statement->name, getContext(), superclass, context));
    deleteScope();
    setVariable(statement->name, classScope);
}