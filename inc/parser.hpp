#pragma once

#include "source_code.hpp"
#include "type.hpp"

struct Parser {
    private:
    std::unordered_set<int> operators = {'.', '+', '-', '*', '/', '^', '%', '=', '<', '>', '!', '|', '&'};

    std::string identifier_str; // Filled in if tok_identifier
    int num_val;             // Filled in if tok_number
    std::string str_val; // Filled in if tok_string

    std::stack<Value> values;
    std::map<std::string, SourceCode> functions;
    std::map<std::string, Value> variables;

    std::map<std::string, Token> command_to_token = {
        {"print", tok_print},
        {"func", tok_func},
        {"var", tok_var},
        {"loop", tok_loop},
        {"break", tok_break},
        {"if", tok_if}
    };

    public:

    int gettok(SourceCode &src);

    int parse(SourceCode &src);
 
    std::stack<Value> get_stack() {
        return values;
    }

    Value try_peek(){
        if (values.empty())
            throw std::runtime_error("Cannot peek empty stack");
        return values.top();
    }

};
