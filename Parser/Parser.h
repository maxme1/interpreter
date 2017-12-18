#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H

#include <vector>
#include <initializer_list>
#include <iostream>
#include <cassert>
#include "Expression/Expression.h"

class Parser {
    typedef std::initializer_list<Token::tokenType> TokenTypes;
//    exceptions
    class ProgramEnd {
    };

    std::vector<Token> tokens;

    bool matches(TokenTypes types);
    Token advance();
    Token require(TokenTypes types);

    Statement *statement() {
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

    Statement *statementBody() {
        if (matches({Token::RETURN}))
            return returnStatement();
        if (matches({Token::RAISE}))
            return raiseStatement();
        if (matches({Token::IMPORT}))
            return importStatement();
        if (matches({Token::BREAK, Token::CONTINUE})) {
            auto control = advance();
            return new ControlFlow(control);
        }
        return new ExpressionStatement(expression());
    }

    Statement *returnStatement() {
        auto token = require({Token::RETURN});
        if (!matches({Token::DELIMITER}))
            return new ReturnStatement(token, expression());
        return new ReturnStatement(token);
    }

    Statement *raiseStatement() {
        require({Token::RAISE});
        return new RaiseStatement(expression());
    }

    Statement *importStatement() {
        require({Token::IMPORT});
        return new ImportStatement(require({Token::IDENTIFIER}).body);
    }

    Statement *ifStatement() {
        require({Token::IF});
        require({Token::BRACKET_OPEN});
        auto condition = expression();
        require({Token::BRACKET_CLOSE});
//        empty if
        if (matches({Token::DELIMITER})) {
            advance();
            return new IfStatement(condition);
        }
        auto left = statement();
//        only left block
        if (!matches({Token::ELSE}))
            return new IfStatement(condition, left);
//        both blocks
        require({Token::ELSE});
        auto right = statement();
        return new IfStatement(condition, left, right);
    }

    Statement *tryStatement() {
        require({Token::TRY});
        auto mainBody = block();
        auto catches = std::vector<TryStatement::CatchStatement *>();
        while (matches({Token::CATCH})) {
            advance();
            auto args = arguments();
            auto body = block();
            catches.push_back(new TryStatement::CatchStatement(args, body));
        }
        return new TryStatement(catches, mainBody);
    }

    Statement *whileStatement() {
        require({Token::WHILE});
        require({Token::BRACKET_OPEN});
        auto condition = expression();
        require({Token::BRACKET_CLOSE});
//        empty body
        if (matches({Token::DELIMITER})) {
            advance();
            return new WhileStatement(condition);
        }
        auto body = statement();
        return new WhileStatement(condition, body);
    }

//    TODO: this function got too large
    Statement *functionDefinition() {
        require({Token::FUNCTION});
        auto name = require({Token::IDENTIFIER}).body;
        require({Token::BRACKET_OPEN});
        auto arguments = std::vector<std::string>();
        bool first = true, unlimited = false;
        while (!matches({Token::BRACKET_CLOSE})) {
            if (!first)
                require({Token::SEPARATOR});
            if (matches({Token::MUL})) {
//                unlimited arguments
                advance();
                unlimited = true;
            }
            auto local = require({Token::IDENTIFIER});
            //        checking uniqueness
            for (auto &argument : arguments) {
                if (argument == local.body)
                    throw "Duplicate argument";
            }

            arguments.push_back(local.body);
            if (unlimited)
                break;
            first = false;
        }
        require({Token::BRACKET_CLOSE});
        auto body = block();
        return new FunctionDefinition(name, arguments, body, unlimited);
    }

    Statement *classDefinition() {
        require({Token::CLASS});
        auto name = require({Token::IDENTIFIER}).body;
        Expression *superclass = nullptr;
        if (matches({Token::BRACKET_OPEN})) {
            advance();
            if (!matches({Token::BRACKET_CLOSE}))
                superclass = expression();
            require({Token::BRACKET_CLOSE});
        }
        return new ClassDefinition(name, block(), superclass);
    }

    Statement *variableDefinition() {
        require({Token::VAR});
        auto name = require({Token::IDENTIFIER}).body;
        Expression *assignee = nullptr;
        if (matches({Token::ASSIGNMENT})) {
            advance();
            assignee = expression();
        }
        require({Token::DELIMITER});
        return new VariableDefinition(name, assignee);
    }

    Block *block() {
        require({Token::BLOCK_OPEN});
        auto statements = std::vector<Statement *>();
        while (!matches({Token::BLOCK_CLOSE}))
            statements.push_back(statement());
        require({Token::BLOCK_CLOSE});
        return new Block(statements);
    };

    Expression *expression() {
        auto left = comparison();
        if (matches({Token::ASSIGNMENT})) {
            Token previous = *(position - 1);
            auto token = advance();
            auto right = expression();
            if (left->ofType(Token::IDENTIFIER)) {
                return new SetVariable(token, previous.body, right);
            }
            if (left->ofType(Token::ATTRIBUTE)) {
                auto lookup = dynamic_cast<GetAttribute *>(left);
                assert(lookup);
                return new SetAttribute(token, lookup, right);
            }
            if (left->ofType(Token::ITEM_OPEN)) {
                auto lookup = dynamic_cast<GetItem *>(left);
                assert(lookup);
                return new SetItem(token, lookup, right);
            }

            assert(false);
//            throw "Bad assignment";
        }
//        here may be a leak
        return left;
    }

    Expression *comparison() {
        auto left = term();
        while (matches({Token::EQUAL, Token::GREATER, Token::GREATER_OR_EQUAL, Token::LESS,
                        Token::LESS_OR_EQUAL, Token::NOT_EQUAL})) {
            auto current = advance();
            auto right = term();
            left = new Binary(current, left, right);
        }
        return left;
    }

    Expression *term() {
        auto left = factor();
        while (matches({Token::ADD, Token::SUB})) {
            auto current = advance();
            auto right = factor();
            left = new Binary(current, left, right);
        }
        return left;
    }

    Expression *factor() {
        auto left = unary();
        while (matches({Token::MUL, Token::DIV})) {
            auto current = advance();
            auto right = unary();
            left = new Binary(current, left, right);
        }
        return left;
    }

    Expression *unary() {
        if (!matches({Token::ADD, Token::SUB}))
            return primary();
        auto current = advance();
        auto argument = primary();
        return new Unary(current, argument);
    }

    Expression *primary() {
        auto left = literal();
        while (matches({Token::BRACKET_OPEN, Token::ATTRIBUTE, Token::ITEM_OPEN})) {
            if (matches({Token::BRACKET_OPEN})) {
                auto token = *position;
                auto args = arguments();
                left = new CallExpression(token, left, args);
            } else if (matches({Token::ATTRIBUTE})) {
                auto token = advance();
                auto name = require({Token::IDENTIFIER}).body;
                left = new GetAttribute(token, left, name);
            } else if (matches({Token::ITEM_OPEN})) {
                auto token = advance();
                auto arg = expression();
                left = new GetItem(token, left, arg);
                require({Token::ITEM_CLOSE});
            }
        }
        return left;
    }

    std::vector<Expression *> arguments() {
        require({Token::BRACKET_OPEN});
        auto result = std::vector<Expression *>();
        while (!matches({Token::BRACKET_CLOSE})) {
            result.push_back(expression());
            if (!matches({Token::BRACKET_CLOSE}))
                require({Token::SEPARATOR});
        }
        advance();
        return result;
    }

    Expression *superClass() {
        require({Token::SUPER});
        require({Token::ATTRIBUTE});
        return new SuperClass(require({Token::IDENTIFIER}));
    }

    Expression *literal() {
        if (matches({Token::SUPER}))
            return superClass();
        if (matches({Token::NUMBER, Token::BOOL, Token::NONE, Token::STRING}))
            return new Literal(advance());
        if (matches({Token::IDENTIFIER}))
            return new Variable(advance());
        auto bracket = require({Token::BRACKET_OPEN});
        auto result = expression();
        require({Token::BRACKET_CLOSE});
        return new Unary(bracket, result);
    }

public:
    bool error = false;
    std::string message{};
    std::vector<Token>::iterator position;

    explicit Parser(const std::vector<Token> &tokens);

//    TODO: no memory is being freed whatsoever
//    TODO: combine build and block
    std::vector<Statement *> build() {
        auto statements = std::vector<Statement *>();
        try {
            while (position != tokens.end())
                statements.push_back(statement());
        } catch (ProgramEnd &e) {
            error = true;
            message = "Unexpected end of file";
        } catch (Token &token) {
            error = true;
            message = "Unexpected token \"" + token.body + "\" at  " +
                      std::to_string(token.line) + ":" + std::to_string(token.column);
        }
        return statements;
    };
};

#endif //INTERPRETER_PARSER_H
