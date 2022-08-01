#pragma once

#include "sourcecode.hpp"
#include "token.hpp"

class Lexer
{
    char c;
    SourceCode source;

public:
    Lexer(SourceCode &src)
    {
        source = src;
        c = ' ';
    }

    Token getNextToken()
    {
        while (isspace(c)) // skip whitespace
        {
            c = source.get_char();
        }

        if (c == -1 || c == '\0')
            return Token(Token::T_EOF);

        if (std::isdigit(c))
        {
            int val = c - '0';
            c = source.get_char();
            while (std::isdigit(c))
            {
                val = val * 10 + c - '0';
                c = source.get_char();
            }
            return Token(Token::NUMBER, val);
        }
        if (std::isalpha(c))
        {
            std::string id;
            while (std::isalnum(c))
                id += c, c = source.get_char();
            return Token(Token::IDENTIFIER, 0, id);
        }
        Token res = Token(Token::T_UNKNOWN);
        switch (c)
        {
        case '*':
            res = Token(Token::MUL);
            break;
        case '/':
            res = Token(Token::DIV);
            break;
        case '+':
            res = Token(Token::ADD);
            break;
        case '-':
            res = Token(Token::SUB);
            break;
        case '(':
            res = Token(Token::LPAREN);
            break;
        case ')':
            res = Token(Token::RPAREN);
            break;
        case '=':
            res = Token(Token::ASSIGN);
            break;
        default:
            break;
        }
        c = source.get_char();
        return res;
    }

    std::string getLocation()
    {
        return "[Ln " + std::to_string(source.get_line_number()) + ", Col" + std::to_string(source.get_column_number()) + "]";
    }

    ~Lexer() = default;
};