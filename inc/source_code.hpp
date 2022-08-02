#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <map>
#include <stack>
#include <vector>
#include <cmath>
#include <unordered_set>

enum Token
{
    tok_eof = -1,

    // commands
    tok_print = -2,
    tok_func = -3,
    tok_var = -4,

    // primary
    tok_identifier = -5,
    tok_number = -6,
};

struct SourceCode
{
    private:
    std::string raw;
    int idx = 0;
    std::vector<std::string> arg_names;

    public:
    SourceCode(std::string &raw_in)
    {
        raw = std::move(raw_in);
        arg_names = {};
    }

    SourceCode(std::string &raw_in, std::vector<std::string> &arg_names_in)
    {
        raw = std::move(raw_in);
        arg_names = std::move(arg_names_in);
    }

    // for some reason a constructor with no arguments gets called, and this is working fine :/
    SourceCode() = default;

    char get_char()
    {
        if (idx == raw.length())
            return EOF;
        return raw[idx++];
    }

    std::vector<std::string> get_arg_names() 
    {
        return arg_names;
    }

    void reset_idx()
    {
        idx = 0;
    }

    std::string replace_args(std::vector<std::string> args);
};
