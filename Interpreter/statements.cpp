#include <iostream>
#include "Interpreter.h"
#include "../Parser/Parser.h"
#include "../Object/Native/Native.h"
#include "../Object/Exception.h"

void Interpreter::evaluate(ReturnStatement *statement) {
    if (statement->expression)
        throw ReturnException(statement->expression->evaluate(this));
    throw ReturnException();
}

void Interpreter::evaluate(RaiseStatement *statement) {
    throw Wrap(statement->expression->evaluate(this));
}

void Interpreter::evaluate(ImportStatement *statement) {
    auto path = statement->path;
//    TODO: for now the imported file must be in the same directory with the executable
    Interpreter source = Interpreter();
    if (!source.interpretFile(path))
        throw Wrap(new ImportError("Could not import " + path));
    setVariable(path, source.getContext());
}

void Interpreter::evaluate(ExpressionStatement *statement) {
    statement->expression->evaluate(this);
}

void Interpreter::evaluate(Statement *statement) {
}

void Interpreter::evaluate(IfStatement *statement) {
    auto cond = statement->condition->evaluate(this);
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
        auto theClass = std::dynamic_pointer_cast<Class>(e.exception);
        if (!theClass) {
            auto instance = std::dynamic_pointer_cast<Instance>(e.exception);
            if (instance)
                theClass = instance->getClass();
        }
        assert(theClass);

        for (auto &&item : statement->catches) {
            auto variants = evaluateArguments(item->arguments);
            for (auto &&variant : variants) {
//                TODO: throw here
                auto arg = std::dynamic_pointer_cast<Class>(variant);
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
    auto cond = statement->condition->evaluate(this);
    while (cond->asBool()) {
        try {
            if (statement->body)
                statement->body->evaluate(this);
        } catch (BreakException) {
            break;
        } catch (ContinueException) {}
        cond = statement->condition->evaluate(this);
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
                New(Function(statement->arguments, statement->body, statement->unlimited, getContext())));
}

void Interpreter::evaluate(ClassDefinition *statement) {
    Class::ptr superclass;
    if (statement->superclass) {
        auto temp = statement->superclass->evaluate(this);
        superclass = std::dynamic_pointer_cast<Class>(temp);
        if (!superclass)
            throw Wrap(new ValueError("Cannot subclass instances"));
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
    auto classScope = std::make_shared<Class>(statement->name, getContext(), superclass, context);
    deleteScope();
    setVariable(statement->name, classScope);
}