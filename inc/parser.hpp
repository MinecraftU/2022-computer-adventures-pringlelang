#pragma once

#include "source_code.hpp"

struct Parser {
    private:
    std::unordered_set<int> operators = {'+', '-', '*', '/', '^', '%', '=', '<', '>', '!', '|', '&'};

    std::string identifier_str; // Filled in if tok_identifier
    int num_val;             // Filled in if tok_number

    std::stack<int> tokens;
    std::map<std::string, SourceCode> functions;
    std::map<std::string, int> variables;

    std::map<std::string, Token> command_to_token = {
        {"print", tok_print},
        {"FUNC", tok_func},
        {"VAR", tok_var},
        {"LOOP", tok_loop},
        {"BREAK", tok_break},
        {"IF", tok_if}
    };

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
