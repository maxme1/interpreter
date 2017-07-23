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
        auto expr = expression();
        require({Token::DELIMITER});
        return new ExpressionStatement(expr);
    };

    Expression *expression() {
        Expression *left = term();
        if (matches({Token::EQUALS})) {
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
        if (matches({Token::NUMBER})) {
//            TODO: create constructor with token
            auto current = advance();
            return new NumberToken(current.body, std::atoi(current.body.c_str()));
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

    std::vector<Statement *> build() {
        auto statements = std::vector<Statement *>();
        while (position != tokens.end())
            statements.push_back(statement());
        return statements;
    };
};


#endif //INTERPRETER_PARSER_H
