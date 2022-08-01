#pragma once

#include <string>
#include <fstream>
#include <streambuf>

class SourceCode
{
public:
    explicit SourceCode(std::ifstream &src)
    {
        source = std::move(std::string(std::istreambuf_iterator<char>(src), std::istreambuf_iterator<char>()));
        line_number = 1;
        column_number = 1;
    }

    SourceCode() = default;

    char get_char()
    {
        if (idx == source.size())
            return -1;
        char c = source[++idx];
        if (c == '\n')
        {
            line_number++;
            column_number = 0;
        }
        else
        {
            column_number++;
        }
        return c;
    }

    int get_line_number()
    {
        return line_number;
    }
    int get_column_number()
    {
        return column_number;
    }

    ~SourceCode() = default;

private:
    int line_number;
    int column_number;
    int idx = -1;
    std::string source;
};