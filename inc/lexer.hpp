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

    Token getNextToken();

    std::string getLocation()
    {
        return "[Ln " + std::to_string(source.get_line_number()) + ", Col" + std::to_string(source.get_column_number()) + "]";
    }

    ~Lexer() = default;
};