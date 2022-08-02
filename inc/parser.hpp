#pragma once

#include "../inc/source_code.hpp"

struct Parser {
    private:
    std::unordered_set<int> operators = {'+', '-', '*', '/', '^', '%'};

    std::string identifier_str; // Filled in if tok_identifier
    double num_val;             // Filled in if tok_number

    std::stack<int> tokens;
    std::map<std::string, SourceCode> functions;
    std::map<std::string, int> variables;

    public:

    int gettok(SourceCode &src);

    int parse(SourceCode &src);
 
    std::stack<int> get_stack() {
        return tokens;
    }

    int try_peek(){
        if (tokens.empty())
            throw std::runtime_error("Cannot peek empty stack");
        return tokens.top();
    }

};