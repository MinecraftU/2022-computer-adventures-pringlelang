#pragma once

#include "source_code.hpp"
#include "type.hpp"

struct Parser {
    private:
    std::unordered_set<int> operators = {'.', '+', '-', '*', '/', '^', '%', '=', '<', '>', '!', '|', '&'};

    std::string identifier_str; // Filled in if tok_identifier
    int num_val;             // Filled in if tok_number
    std::string str_val; // Filled in if tok_string

    std::stack<Value> stack;
    std::unordered_map<std::string, SourceCode> functions;
    std::unordered_map<std::string, Value> variables;

    std::unordered_map<std::string, Token> command_to_token = {
        {"print", tok_print},
        {"func", tok_func},
        {"var", tok_var},
        {"loop", tok_loop},
        {"break", tok_break},
        {"if", tok_if},
        {"dup", tok_dup},
        {"swap", tok_swap},
        {"over", tok_over},
        {"twodup", tok_twodup}
    };

    public:

    int gettok(SourceCode &src);

    int parse(SourceCode &src);
 
    std::stack<Value> get_stack() {
        return stack;
    }

    Value try_peek(){
        if (stack.empty())
            throw std::runtime_error("Cannot peek empty stack");
        return stack.top();
    }

};
