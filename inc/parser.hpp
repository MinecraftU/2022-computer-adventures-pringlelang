#pragma once

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

    ASTNode *parse();
    ASTNode *expr();
    ASTNode *term();
    ASTNode *factor();

    void eat(Token::Type type); 
};