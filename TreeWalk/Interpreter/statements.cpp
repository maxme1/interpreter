#include <iostream>
#include "Interpreter.h"
#include "../../Parser/Parser.h"
#include "../../Object/Native/Native.h"
#include "../../Object/Exception.h"
#include "../../Object/Types/Int.h"
#include "../../Object/Class.h"
#include "../../Object/Types/None.h"

void Interpreter::visit(ReturnStatement *statement) {
    if (statement->expression)
        throw ReturnException(statement->expression->visit(this));
    throw ReturnException();
}

void Interpreter::visit(RaiseStatement *statement) {
    assert(statement->expression);
//    TODO: implement reraising
    throw ExceptionWrapper(statement->expression->visit(this));
}

void Interpreter::visit(ImportStatement *statement) {
    assert(false);
    auto path = statement->path;
//    TODO: for now the imported file must be in the same directory with the executable
    Interpreter source = Interpreter();
//    if (!source.interpretFile(path))
//        throw Wrap(new ImportError("Could not import " + path));
//    setVariable(path, source.getClosure(), 0);
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
    try {
        statement->block->visit(this);
    } catch (ExceptionWrapper &e) {
        auto theClass = e.exception->getClass();
        assert(theClass);

        for (auto &&item : statement->catches) {
            auto exceptions = evaluateArguments(item->arguments);
            for (auto &&exception : exceptions) {
                auto exClass = std::dynamic_pointer_cast<Class>(exception);
                if (exClass == nullptr)
                    throw ExceptionWrapper(new ValueError("Trying to catch something different from a class"));
                if (isDerived(theClass, exClass)) {
                    item->block->visit(this);
                    return;
                }
            }
        }
//        TODO: add suppressing
        if (!statement->catches.empty())
            throw;
    }
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
    if (statement->token.type == Token::CONTINUE)
        throw ContinueException();
    throw BreakException();
}

void Interpreter::visit(Block *block) {
    enterScope();
    try {
        visitStatements(block->statements);
    } catch (ExceptionWrapper &e) {
        leaveScope();
        throw;
    } catch (FlowException &e) {
        leaveScope();
        throw;
    }
    leaveScope();
}

void Interpreter::visit(FunctionDefinition *statement) {
    std::vector<FunctionPrototype::Argument> arguments;
    for (auto &&argument :statement->arguments) {
        ObjPtr default_ = nullptr;
        if (argument.defaultValue != nullptr)
            default_ = argument.defaultValue->visit(this);
        arguments.emplace_back(argument.name, default_, argument.positional, argument.variable);
    }

    defineVariable(statement->name, New(Function(statement->body, getClosure(), arguments)));
}

void Interpreter::visit(VariableDefinition *statement) {
    if (statement->assignee)
        defineVariable(statement->name, statement->assignee->visit(this));
    else
        defineVariable(statement->name, New(None()));
}

void Interpreter::visit(ClassDefinition *statement) {
    Class::ptr superclass;
    if (statement->superclass) {
        auto temp = statement->superclass->visit(this);
        superclass = std::dynamic_pointer_cast<Class>(temp);
        if (not superclass)
            throw ExceptionWrapper(new ValueError("The provided superclass is not a class"));
    }
    auto closure = getClosure();
    enterScope();

    if (statement->superclass)
        defineVariable("super", superclass);
    visitStatements(statement->body->statements);
    auto theClass = std::make_shared<Class>(statement->name, getClosure(), superclass, closure);

    leaveScope();
    defineVariable(statement->name, theClass);
}