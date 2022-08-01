#include "interpreter.hpp"

int Interpreter::visit(ASTNode *node)
{
    switch (node->token.getType())
    {
    case Token::NUMBER:
    {
        NumNode *num = static_cast<NumNode *>(node);
        return num->token.getValue();
    }
    case Token::ADD:
    {
        BinOpNode *binop = static_cast<BinOpNode *>(node);
        return visit(binop->left) + visit(binop->right);
    }
    case Token::SUB:
    {
        BinOpNode *binop = static_cast<BinOpNode *>(node);
        return visit(binop->left) - visit(binop->right);
    }
    case Token::UNARY_SUB:
    {
        UnaryOpNode *unop = static_cast<UnaryOpNode *>(node);
        return -visit(unop->expr);
    }
    case Token::MUL:
    {
        BinOpNode *binop = static_cast<BinOpNode *>(node);
        return visit(binop->left) * visit(binop->right);
    }
    case Token::DIV:
    {
        BinOpNode *binop = static_cast<BinOpNode *>(node);
        try
        {
            return visit(binop->left) / visit(binop->right);
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("cannot divide by zero");
        }
    }
    default:
        throw std::runtime_error("Error while interpreting AST: " + node->token.getId() + " expected");
    }
}