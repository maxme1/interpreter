#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H


#include <vector>
#include <initializer_list>
#include <iostream>
#include "Expression/Expression.h"

class Parser {
    typedef std::initializer_list<Token::tokenType> TokenTypes;
    std::vector<Token> tokens;
    std::vector<Token>::iterator position;

    bool matches(std::initializer_list<Token::tokenType> types);
    Token advance();
    Token require(std::initializer_list<Token::tokenType> types);

    Statement *statement() {
        if (matches({Token::BLOCK_OPEN}))
            return block();
        if (matches({Token::IF}))
            return ifStatement();
        if (matches({Token::WHILE}))
            return whileStatement();
        if (matches({Token::FUNCTION}))
            return functionDefinition();

        auto body = statementBody();
        require({Token::DELIMITER});
        return body;
    };

    Statement *statementBody() {
        if (matches({Token::RETURN}))
            return returnStatement();
        if (matches({Token::BREAK, Token::CONTINUE})) {
            auto control = advance();
            return new ControlFlow(control.type, control.body);
        }
        return new ExpressionStatement(expression());
    }

    Statement *returnStatement() {
        require({Token::RETURN});
        if (!matches({Token::DELIMITER}))
            return new ReturnStatement(expression());
        return new ReturnStatement();
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
        advance();
        auto right = statement();
        return new IfStatement(condition, left, right);
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

    Statement *functionDefinition() {
        require({Token::FUNCTION});
        auto name = require({Token::IDENTIFIER}).body;
        require({Token::BRACKET_OPEN});
        auto arguments = std::vector<std::string>();
        bool first = true;
        while (!matches({Token::BRACKET_CLOSE})) {
            if (!first)
                require({Token::SEPARATOR});
            auto local = require({Token::IDENTIFIER});
            arguments.push_back(local.body);
            first = false;
        }
        require({Token::BRACKET_CLOSE});
        auto body = block();
        return new FunctionDefinition(name, arguments, body);
    }

    Statement *block() {
        require({Token::BLOCK_OPEN});
        auto statements = std::vector<Statement *>();
        while (position != tokens.end() and !matches({Token::BLOCK_CLOSE}))
            statements.push_back(statement());
        require({Token::BLOCK_CLOSE});
        return new Block(statements);
    };

    Expression *expression() {
        Expression *left = comparison();
        if (matches({Token::ASSIGNMENT})) {
            Token previous = *(position - 1);
            advance();
            Expression *right = expression();
            if (left->ofType(Token::IDENTIFIER)) {
                return new SetVariable(previous.body, right);
            }

            throw "Bad assignment";
        }
        return left;
    }

    Expression *comparison() {
        Expression *left = term();
        while (matches({Token::EQUAL, Token::GREATER, Token::GREATER_OR_EQUAL, Token::LESS, Token::LESS_OR_EQUAL,
                        Token::NOT_EQUAL})) {
            auto current = advance();
            Expression *right = term();
            left = new Binary(current.body, current.type, left, right);
        }
        return left;
    }

    Expression *term() {
        Expression *left = factor();
        while (matches({Token::ADD, Token::SUB})) {
            auto current = advance();
            Expression *right = factor();
            left = new Binary(current.body, current.type, left, right);
        }
        return left;
    }

    Expression *factor() {
        Expression *left = unary();
        while (matches({Token::MUL, Token::DIV})) {
            auto current = advance();
            Expression *right = unary();
            left = new Binary(current.body, current.type, left, right);
        }
        return left;
    }

    Expression *unary() {
        if (!matches({Token::ADD, Token::SUB}))
            return primary();
        auto current = advance();
        Expression *argument = primary();
        return new Unary(current.body, current.type, argument);
    }

    Expression *primary() {
        auto left = literal();
//        function
        while (matches({Token::BRACKET_OPEN})) {
            advance();
            auto args = arguments();
            left = new FunctionExpression(left, args);
        }
        return left;
    }

    std::vector<Expression *> arguments() {
        auto result = std::vector<Expression *>();
        while (!matches({Token::BRACKET_CLOSE})) {
            result.push_back(expression());
            if (!matches({Token::BRACKET_CLOSE}))
                require({Token::SEPARATOR});
        }
        advance();
        return result;
    }

    Expression *literal() {
        if (matches({Token::NUMBER, Token::BOOL})) {
//            TODO: create constructor with token
            auto current = advance();
            return new Literal(current.body, current.type);
        }
        if (matches({Token::IDENTIFIER})) {
            auto current = advance();
            return new Variable(current.body, current.type);
        }
        require({Token::BRACKET_OPEN});
        auto result = expression();
        require({Token::BRACKET_CLOSE});
        return new Unary("()", Token::BRACKET, result);
    }

public:
    bool error = false;

    Parser(const std::vector<Token> &tokens);

//    TODO: combine build and block
    std::vector<Statement *> build() {
        auto statements = std::vector<Statement *>();
        try {
            while (position != tokens.end())
                statements.push_back(statement());
        } catch (TokenTypes types) {
            error = true;
//            TODO: no memory is being freed whatsoever
//            TODO: more specific
            std::cout << "Expected something else instead of " << position->body;
        } catch (char const *message) {
            error = true;
            std::cout << message;
        }
        return statements;
    };
};


#endif //INTERPRETER_PARSER_H
