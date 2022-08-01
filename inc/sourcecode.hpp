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

    explicit SourceCode(std::string &str){
        source = std::move(str);
        line_number = 1;
        column_number = 1;
    }

    SourceCode() = default;

    char get_char();
    

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