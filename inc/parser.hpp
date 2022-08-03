#pragma once
#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "../inc/source_code.hpp"

struct Parser {
    private:

    std::string identifier_str; // Filled in if tok_identifier
    int num_val;             // Filled in if tok_number

    std::stack<int> tokens;
    std::map<std::string, SourceCode> functions;
    std::map<std::string, int> variables;

    public:

    int gettok(SourceCode &src);

    int parse(SourceCode &src);
 
    std::stack<int> get_stack() {
        return tokens;
    }
};

#endif // _PARSER_HPP_
