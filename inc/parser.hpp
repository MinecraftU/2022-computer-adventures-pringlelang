#pragma once

#include "parser.hpp"
#include "token.hpp"
#include "lexer.hpp"

// AST node class
class ASTNode
{
public:
    Token token;
    ASTNode(Token token) : token(token) {}
    virtual ~ASTNode() = default;
};

// Binary operator node class
class BinOpNode : public ASTNode
{
public:
    ASTNode *left;
    ASTNode *right;
    BinOpNode(const Token &token, ASTNode *left, ASTNode *right) : ASTNode(token), left(left), right(right) {}
};

class UnaryOpNode : public ASTNode
{
public:
    ASTNode *expr;
    UnaryOpNode(const Token &token, ASTNode *ex) : ASTNode(token), expr(ex) {}
};

// Number node class
class NumNode : public ASTNode
{
public:
    NumNode(Token &token) : ASTNode(token) {}
};

// Parser class
class Parser
{
    Lexer *lexer;
    Token current_token = Token(Token::T_UNKNOWN);

public:
    Parser(Lexer *lex)
    {
        lexer = lex;
        current_token = lexer->getNextToken();
    }

    // Compare current token type with the given token type and if they match then "eat"
    // the current token and assign the next token to current_token, otherwise raise an error.
    void eat(Token::Type type)
    {
        if (current_token.getType() == type)
        {
            current_token = lexer->getNextToken();
        }
        else
        {
            throw std::runtime_error("Syntax error: type " + Token::getTypeName(type) + " expected, but found" + Token::getTypeName(current_token.getType()));
        }
    }

    // factor : NUMBER | LPAREN expr RPAREN | (PLUS | MINUS) factor
    // TODO: IDENTIFIER
    ASTNode *factor()
    {
        if (current_token.getType() == Token::SUB)
        {
            Token t = current_token;
            eat(Token::SUB);
            t.setType(Token::UNARY_SUB);
            return new UnaryOpNode(t, factor());
        }
        else if (current_token.getType() == Token::NUMBER)
        {
            Token t = current_token;
            eat(Token::NUMBER);
            return new NumNode(t);
        }
        else if (current_token.getType() == Token::LPAREN)
        {
            eat(Token::LPAREN);
            ASTNode *node = expr();
            eat(Token::RPAREN);
            return node;
        }
        else
        {
            throw std::runtime_error(
                "Syntax error" + lexer->getLocation() +
                ": NUMBER expected as factor, instead " + Token::getTypeName(current_token.getType()) + " was found");
        }
    }

    // term : factor  ((MUL | DIV) factor)*
    // EXAMPLE 1: term : factor MUL factor
    // EXAMPLE 2: term : factor MUL factor DIV factor
    // EXAMPLE 3: term : factor
    ASTNode *term()
    {
        ASTNode *left = factor();
        while (current_token.getType() == Token::MUL || current_token.getType() == Token::DIV)
        {
            Token t = current_token;
            if (t.getType() == Token::MUL)
                eat(Token::MUL);
            else if (t.getType() == Token::DIV)
                eat(Token::DIV);
            ASTNode *right = factor();
            left = new BinOpNode(t, left, right);
        }
        return left;
    }

    // expr : term ((ADD | SUB) term)*
    // EXAMPLE 1: expr : term ADD term
    // EXAMPLE 1 with more details: expr : factor MUL factor ADD factor DIV factor
    // EXAMPLE 1 with even more details: expr : 1 MUL 2 ADD 4 DIV 2
    //                                          (1 * 2) + (4 / 2) = 4
    // EXAMPLE 2: expr : term ADD term SUB term
    // EXAMPLE 2 with more details: expr : factor MUL factor ADD factor DIV factor SUB factor
    // EXAMPLE 2 with even more details: expr : 1 MUL 2 ADD 4 DIV 2 SUB 1
    //                                          1 * 2 + 4 / 2 - 1
    //                                          ((1 * 2) + (4 / 2)) - 1 = 3
    ASTNode *expr()
    {
        ASTNode *left = term();

        while (current_token.getType() == Token::ADD || current_token.getType() == Token::SUB)
        {
            Token t = current_token;
            if (t.getType() == Token::ADD)
                eat(Token::ADD);
            else if (t.getType() == Token::SUB)
                eat(Token::SUB);
            ASTNode *right = term();
            left = new BinOpNode(t, left, right);
        }
        return left;
    }

    // parse
    ASTNode *parse()
    {
        return expr();
    }
};