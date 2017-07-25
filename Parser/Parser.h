#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H


#include <vector>
#include <initializer_list>
#include <iostream>
#include "Expression/Expression.h"

class Parser {
    std::vector<Token> tokens;
    std::vector<Token>::iterator position;
    bool error = false;

    bool matches(std::initializer_list<Token::tokenType> types);

    Token advance();

    Token require(std::initializer_list<Token::tokenType> types);

    Statement *statement() {
        if (matches({Token::BLOCK_OPEN}))
            return new Block(block());
        auto expr = expression();
        require({Token::DELIMITER});
        return new ExpressionStatement(expr);
    };

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

            return nullptr;
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
//        TODO: raise something
        require({Token::BRACKET_OPEN});
        auto result = expression();
        require({Token::BRACKET_CLOSE});
        return new Unary("()", Token::BRACKET, result);
    }

public:
    Parser(const std::vector<Token> &tokens);

//    TODO: combine build and block
    std::vector<Statement *> build() {
        auto statements = std::vector<Statement *>();
        while (position != tokens.end())
            statements.push_back(statement());
        return statements;
    };
};


#endif //INTERPRETER_PARSER_H
