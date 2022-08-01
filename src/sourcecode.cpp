#include "sourcecode.hpp"

char SourceCode::get_char()
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