#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H

#include <vector>
#include <initializer_list>
#include <iostream>
#include <cassert>
#include "Expression/Expression.h"
#include "../Object/Exception.h"

class Parser {
    typedef std::initializer_list<Token::tokenType> TokenTypes;
    struct SyntaxError : public BaseExceptionWrapper {
        explicit SyntaxError(const std::string &message) : BaseExceptionWrapper(message) {}
    };

    std::vector<Token> tokens;

    bool matches(TokenTypes types, int shift = 0);
    Token advance();
    Token require(TokenTypes types);

    Statement::ptr statement() {
        if (matches({Token::BLOCK_OPEN}))
            return block();
        if (matches({Token::IF}))
            return ifStatement();
        if (matches({Token::WHILE}))
            return whileStatement();
        if (matches({Token::TRY}))
            return tryStatement();
        if (matches({Token::FUNCTION}))
            return functionDefinition();
        if (matches({Token::CLASS}))
            return classDefinition();
        if (matches({Token::VAR}))
            return variableDefinition();

        auto body = statementBody();
        require({Token::DELIMITER});
        return body;
    };

    Statement::ptr statementBody() {
        if (matches({Token::RETURN}))
            return returnStatement();
        if (matches({Token::RAISE}))
            return raiseStatement();
        if (matches({Token::IMPORT}))
            return importStatement();
        if (matches({Token::BREAK, Token::CONTINUE})) {
            auto control = advance();
            return Statement::ptr(new ControlFlow(control));
        }
        return Statement::ptr(new ExpressionStatement(expression()));
    }

    Statement::ptr returnStatement() {
        auto token = require({Token::RETURN});
        if (!matches({Token::DELIMITER}))
            return Statement::ptr(new ReturnStatement(token, expression()));
        return Statement::ptr(new ReturnStatement(token));
    }

    Statement::ptr raiseStatement() {
        require({Token::RAISE});
        return Statement::ptr(new RaiseStatement(expression()));
    }

    Statement::ptr importStatement() {
        auto token = require({Token::IMPORT});
        return Statement::ptr(new ImportStatement(token, require({Token::IDENTIFIER}).body));
    }

    Statement::ptr ifStatement() {
        auto token = require({Token::IF});
        require({Token::BRACKET_OPEN});
        auto condition = expression();
        require({Token::BRACKET_CLOSE});
//        empty if
        if (matches({Token::DELIMITER})) {
            advance();
            return Statement::ptr(new IfStatement(token, condition));
        }
        auto left = statement();
//        only left block
        if (!matches({Token::ELSE}))
            return Statement::ptr(new IfStatement(token, condition, left));
//        both blocks
        require({Token::ELSE});
        auto right = statement();
        return Statement::ptr(new IfStatement(token, condition, left, right));
    }

    Statement::ptr tryStatement() {
        require({Token::TRY});
        auto mainBody = block();
        auto catches = std::vector<shared(TryStatement::CatchStatement) >();
        while (matches({Token::CATCH})) {
            advance();
            require({Token::BRACKET_OPEN});
            auto args = positional();
            require({Token::BRACKET_CLOSE});
            auto body = block();
            catches.emplace_back(new TryStatement::CatchStatement(args, body));
        }
        return Statement::ptr(new TryStatement(catches, mainBody));
    }

    Statement::ptr whileStatement() {
        require({Token::WHILE});
        require({Token::BRACKET_OPEN});
        auto condition = expression();
        require({Token::BRACKET_CLOSE});
//        empty body
        if (matches({Token::DELIMITER})) {
            advance();
            return Statement::ptr(new WhileStatement(condition));
        }
        auto body = statement();
        return Statement::ptr(new WhileStatement(condition, body));
    }

    Statement::ptr functionDefinition() {
        require({Token::FUNCTION});
        auto name = require({Token::IDENTIFIER}).body;
        require({Token::BRACKET_OPEN});
        auto arguments = std::vector<FunctionDefinition::Argument>();
//        positional
        bool positional = true;
        while (!matches({Token::BRACKET_CLOSE})) {
//            if kwarg
            if (matches({Token::IDENTIFIER}) and matches({Token::ASSIGNMENT}, 1))
                break;
            bool variable = false;
            if (matches({Token::ASTERISK})) {
                variable = true;
                advance();
            }
            auto argName = require({Token::IDENTIFIER}).body;
            arguments.emplace_back(argName, nullptr, positional, variable);
            if (variable)
                positional = false;
            if (!matches({Token::BRACKET_CLOSE}))
                require({Token::SEPARATOR});
        }

        auto kw = kwargs();
        for (auto &&argument : kw)
            arguments.emplace_back(argument->name, argument->value, positional, false);

        require({Token::BRACKET_CLOSE});
        auto body = block();
        return Statement::ptr(new FunctionDefinition(name, body, arguments));
    }

    Statement::ptr classDefinition() {
        require({Token::CLASS});
        auto name = require({Token::IDENTIFIER}).body;
        Expression::ptr superclass = nullptr;
        if (matches({Token::BRACKET_OPEN})) {
            advance();
            if (!matches({Token::BRACKET_CLOSE}))
                superclass = expression();
            require({Token::BRACKET_CLOSE});
        }
        return Statement::ptr(new ClassDefinition(name, block(), superclass));
    }

    Statement::ptr variableDefinition() {
        require({Token::VAR});
        auto name = require({Token::IDENTIFIER}).body;
        Expression::ptr assignee = nullptr;
        if (matches({Token::ASSIGNMENT})) {
            advance();
            assignee = expression();
        }
        require({Token::DELIMITER});
        return Statement::ptr(new VariableDefinition(name, assignee));
    }

    shared(Block) block() {
        auto token = require({Token::BLOCK_OPEN});
        auto statements = std::vector<Statement::ptr>();
        while (!matches({Token::BLOCK_CLOSE}))
            statements.push_back(statement());
        require({Token::BLOCK_CLOSE});
        return shared(Block)(new Block(token, statements));
    };

    Expression::ptr expression() {
        auto left = comparison();
        if (matches({Token::ASSIGNMENT})) {
            Token previous = *(position - 1);
            auto token = advance();
            auto right = expression();
            if (left->ofType(Token::IDENTIFIER)) {
                return Expression::ptr(new SetVariable(token, previous.body, right));
            }
            if (left->ofType(Token::ATTRIBUTE)) {
                auto lookup = std::dynamic_pointer_cast<GetAttribute>(left);
                assert(lookup);
                return Expression::ptr(new SetAttribute(token, lookup, right));
            }
            if (left->ofType(Token::ITEM_OPEN)) {
                auto lookup = std::dynamic_pointer_cast<GetItem>(left);
                assert(lookup);
                return Expression::ptr(new SetItem(token, lookup, right));
            }

            assert(false);
//            throw "Bad assignment";
        }
//        here may be a leak
        return left;
    }

    Expression::ptr comparison() {
        auto left = term();
        while (matches({Token::EQUAL, Token::GREATER, Token::GREATER_OR_EQUAL, Token::LESS,
                        Token::LESS_OR_EQUAL, Token::NOT_EQUAL})) {
            auto current = advance();
            auto right = term();
            left = Expression::ptr(new Binary(current, left, right));
        }
        return left;
    }

    Expression::ptr term() {
        auto left = factor();
        while (matches({Token::ADD, Token::SUB})) {
            auto current = advance();
            auto right = factor();
            left = Expression::ptr(new Binary(current, left, right));
        }
        return left;
    }

    Expression::ptr factor() {
        auto left = unary();
        while (matches({Token::ASTERISK, Token::DIV})) {
            auto current = advance();
            auto right = unary();
            left = Expression::ptr(new Binary(current, left, right));
        }
        return left;
    }

    Expression::ptr unary() {
        if (!matches({Token::ADD, Token::SUB}))
            return primary();
        auto current = advance();
        auto argument = primary();
        return Expression::ptr(new Unary(current, argument));
    }

    Expression::ptr primary() {
        auto left = literal();
        while (matches({Token::BRACKET_OPEN, Token::ATTRIBUTE, Token::ITEM_OPEN})) {
            if (matches({Token::BRACKET_OPEN})) {
                auto token = *position;
                require({Token::BRACKET_OPEN});
                auto args = positional();
                auto kw = kwargs();
                require({Token::BRACKET_CLOSE});
                left = Expression::ptr(new CallExpression(token, left, args, kw));
            } else if (matches({Token::ATTRIBUTE})) {
                auto token = advance();
                auto name = require({Token::IDENTIFIER}).body;
                left = Expression::ptr(new GetAttribute(token, left, name));
            } else if (matches({Token::ITEM_OPEN})) {
                auto token = advance();
                auto arg = expression();
                left = Expression::ptr(new GetItem(token, left, arg));
                require({Token::ITEM_CLOSE});
            }
        }
        return left;
    }

    std::vector<Expression::ptr> positional() {
        auto result = std::vector<Expression::ptr>();
        while (!matches({Token::BRACKET_CLOSE})) {
//            if kwarg
            if (matches({Token::IDENTIFIER}) and matches({Token::ASSIGNMENT}, 1))
                break;

            result.push_back(expression());
            if (!matches({Token::BRACKET_CLOSE}))
                require({Token::SEPARATOR});
        }
        return result;
    }

    std::vector<shared(SetVariable) > kwargs() {
        auto result = std::vector<shared(SetVariable) >();
        while (!matches({Token::BRACKET_CLOSE})) {
            auto token = require({Token::IDENTIFIER});
            require({Token::ASSIGNMENT});
            result.emplace_back(new SetVariable(token, token.body, expression()));
            if (!matches({Token::BRACKET_CLOSE}))
                require({Token::SEPARATOR});
        }
        return result;
    }

    Expression::ptr superClass() {
        require({Token::SUPER});
        require({Token::ATTRIBUTE});
        return Expression::ptr(new SuperClass(require({Token::IDENTIFIER})));
    }

    Expression::ptr literal() {
        if (matches({Token::SUPER}))
            return superClass();
        if (matches({Token::NUMBER, Token::BOOL, Token::NONE, Token::STRING}))
            return Expression::ptr(new Literal(advance()));
        if (matches({Token::IDENTIFIER}))
            return Expression::ptr(new Variable(advance()));
        auto bracket = require({Token::BRACKET_OPEN});
        auto result = expression();
        require({Token::BRACKET_CLOSE});
        return Expression::ptr(new Unary(bracket, result));
    }

public:
    std::vector<Token>::iterator position;

    explicit Parser(const std::vector<Token> &tokens);

//    TODO: no memory is being freed whatsoever
//    TODO: combine build and block
    std::vector<Statement::ptr> build() {
        auto statements = std::vector<Statement::ptr>();
        while (position != tokens.end())
            statements.push_back(statement());
        return statements;
    };
};

#endif //INTERPRETER_PARSER_H
