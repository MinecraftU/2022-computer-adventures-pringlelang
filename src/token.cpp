#include "token.hpp"

std::string Token::getTypeName(Type type)
{
    switch (type)
    {
    case NUMBER:
        return "NUMBER";
    case MUL:
        return "MUL";
    case DIV:
        return "DIV";
    case ADD:
        return "ADD";
    case SUB:
        return "SUB";
    case T_EOF:
        return "EOF";
    case LPAREN:
        return "LPAREN";
    case RPAREN:
        return "RPAREN";
    case ASSIGN:
        return "ASSIGN";
    default:
        return "UNKNOWN";
    }
}

// Token stream override
std::ostream &operator<<(std::ostream &Str, Token const &token)
{
    Str << "Token(" << Token::getTypeName(token.type) << ": ";
    if (token.type == Token::NUMBER)
        Str << token.value;
    else if (token.type == Token::IDENTIFIER)
        Str << token.id;
    Str << ")";
    return Str;
}