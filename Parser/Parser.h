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
            return new Block(block());
        if (matches({Token::IF}))
            return ifStatement();
        if (matches({Token::WHILE}))
            return whileStatement();
        if (matches({Token::BREAK, Token::CONTINUE})) {
            auto control = advance();
            require({Token::DELIMITER});
            return new ControlFlow(control.type, control.body);
        }
        auto expr = expression();
        require({Token::DELIMITER});
        return new ExpressionStatement(expr);
    };

    Statement *ifStatement() {
        advance();
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
        advance();
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

    std::vector<Statement *> block() {
        advance();
        auto statements = std::vector<Statement *>();
        while (position != tokens.end() and !matches({Token::BLOCK_CLOSE}))
            statements.push_back(statement());
        require({Token::BLOCK_CLOSE});
        return statements;
    };

    Expression *expression() {
        Expression *left = term();
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
            return literal();
        auto current = advance();
        Expression *argument = literal();
        return new Unary(current.body, current.type, argument);
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
