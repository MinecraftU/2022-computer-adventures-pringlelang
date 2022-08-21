#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <stack>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <unordered_map>

enum Token
{
    tok_eof = -1,

    // commands
    tok_print = -2,
    tok_func = -3,
    tok_var = -4,
    tok_loop = -5,
    tok_break = -6,
    tok_if = -7,

    // primary
    tok_identifier = -8,
    tok_number = -9,
    tok_string = -10,

    //stack commands
    tok_dup = -11,
    tok_swap = -12,
    tok_over = -13,
    tok_twodup = -14,
    tok_pop = -15,
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

    std::string replace_args(const std::vector<std::string>& args);
};
