#include "../inc/parser.hpp"

int Parser::gettok(SourceCode &src)
{
    int last_char = ' ';

    // Skip any whitespace.
    while (isspace(last_char)) {
        last_char = src.get_char();
    }

    if (isalpha(last_char)) // function names can currently be anything; make it so it can only be alphanumeric
    { // identifier: [a-zA-Z][a-zA-Z0-9]*
        identifier_str = last_char;
        while (isalnum((last_char = src.get_char())))
            identifier_str += last_char;

        if (identifier_str == "print")
            return tok_print;
        if (identifier_str == "FUNC")
            return tok_func;
        if (identifier_str == "VAR")
            return tok_var;
        if (identifier_str == "LOOP")
            return tok_loop;
        if (identifier_str == "BREAK")
            return tok_break;
        if (identifier_str == "IF")
            return tok_if;
        return tok_identifier;
    }

    if (isdigit(last_char))
    { // Number: [0-9.]+
        std::string NumStr;
        do
        {
            NumStr += last_char;
            last_char = src.get_char();
        } while (isdigit(last_char));

        num_val = std::stoi(NumStr);
        return tok_number;
    }

    if (last_char == '#')
    {
        // Comment until end of line.
        do
            last_char = src.get_char();
        while (last_char != EOF && last_char != '\n' && last_char != '\r');

        if (last_char != EOF)
            return gettok(src);
    }

    // Check for end of file.  Don't eat the EOF.
    if (last_char == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int this_char = last_char;
    last_char = src.get_char();
    return this_char;
}

int Parser::parse(SourceCode &src)
{
    int token = gettok(src);
    while (token != -1)
    {
        // for functions/operators
        std::vector<int> args;
        // for case tok_func
        int lb_found = 0; // there are 3 left brackets, go from 1 to 2 to 3 when they are found.
        int rb_found = 0; // there are 3 right brackets, go from 1 to 2 to 3 when they are found.
        std::string name = "";
        std::vector<std::string> arg_names;
        std::string arg_name = "";
        std::string inside_src = "";
        // for case tok_loop
        SourceCode new_src;
        switch (token)
        {
        case tok_print:
            args.push_back(tokens.top());
            tokens.pop();
            std::cout << args[0];
            break;
        case tok_func:
            while (lb_found != rb_found || lb_found < 3) {
                char c = src.get_char();
                if (rb_found > lb_found) {
                    std::cout << "Syntax Error: incorrect bracket placement.\n";
                    return 1;
                }
                if (c == '{') lb_found++;
                if (c == '}') rb_found++;
                if (c != '{' && lb_found > rb_found && rb_found == 0) {
                    name += c;
                } else if (lb_found > rb_found && rb_found == 1) {
                    if (c == ' ' && arg_name != "") {
                        arg_names.push_back(arg_name);
                        arg_name = "";
                    } else if (c != '{') {
                        arg_name += c;
                    }
                } else if ((lb_found > rb_found && rb_found == 2) || rb_found > 2) {
                    if (arg_name != "" && arg_name != "{") {
                        arg_names.push_back(arg_name);
                        arg_name = "";
                    }
                    if (c != '{' || lb_found != 3) inside_src += c;
                }
            }
            inside_src.pop_back();
            functions[name] = std::move(SourceCode(inside_src, arg_names));
            break;
        case tok_var:
            while (!lb_found || !rb_found) {
                char c = src.get_char();
                if (rb_found && !lb_found) {
                    std::cout << "Syntax Error: incorrect bracket placement.\n";
                    return 1;
                }
                if (c == '{') lb_found = true;
                else if (c == '}') rb_found = true;
                else if (lb_found) {
                    name += c;
                }
            }
            args.push_back(tokens.top());
            tokens.pop();
            variables[name] = args[0];
            break;
        case tok_loop:
            while (lb_found != rb_found || lb_found < 1) {
                char c = src.get_char();
                if (rb_found > lb_found) {
                    std::cout << "Syntax Error: incorrect bracket placement.\n";
                    return 1;
                }
                if (c == '{') lb_found++;
                if (c == '}') rb_found++;
                if (((lb_found > rb_found && rb_found == 0) || rb_found > 0) &&
                    (c != '{' || lb_found != 1)) {
                    inside_src += c;
                }
            }
            inside_src.pop_back();

            new_src = std::move(SourceCode(inside_src));
            while (true) {
                if (parse(new_src) != 0) break;
                new_src.reset_idx();
            }
        case tok_break:
            return 2;
        case tok_if:
            while (lb_found != rb_found || lb_found < 1) {
                char c = src.get_char();
                if (rb_found > lb_found) {
                    std::cout << "Syntax Error: incorrect bracket placement.\n";
                    return 1;
                }
                if (c == '{') lb_found++;
                if (c == '}') rb_found++;
                if (((lb_found > rb_found && rb_found == 0) || rb_found > 0) &&
                    (c != '{' || lb_found != 1)) {
                    inside_src += c;
                }
            }
            inside_src.pop_back();

            args.push_back(tokens.top());
            tokens.pop();
            if (args[0] > 0) {
                new_src = std::move(SourceCode(inside_src));
                if (parse(new_src) != 0) return 2;
            }
            break;
        case '+':
            args.push_back(tokens.top());
            tokens.pop();
            args.push_back(tokens.top());
            tokens.pop();
            tokens.push(args[1] + args[0]);
            break;
        case '*':
            args.push_back(tokens.top());
            tokens.pop();
            args.push_back(tokens.top());
            tokens.pop();
            tokens.push(args[1] * args[0]);
            break;
        case '-':
            args.push_back(tokens.top());
            tokens.pop();
            args.push_back(tokens.top());
            tokens.pop();
            tokens.push(args[1] - args[0]);
            break;
        case '/':
            args.push_back(tokens.top());
            tokens.pop();
            args.push_back(tokens.top());
            tokens.pop();
            tokens.push(args[1] / args[0]);
            break;
        case tok_identifier:
            if (functions.count(identifier_str)) { // if identifier_str is a key in functions
                functions[identifier_str].reset_idx();
                std::vector<std::string> str_args;
                for (size_t i = 0; i < functions[identifier_str].get_arg_names().size(); i++) {
                    str_args.push_back(std::to_string(tokens.top()));
                    tokens.pop();
                }
                std::reverse(str_args.begin(), str_args.end());

                std::string replaced_raw = functions[identifier_str].replace_args(str_args);
                SourceCode replaced = std::move(SourceCode(replaced_raw));
                parse(replaced);
            } else if (variables.count(identifier_str)) { // if identifier_str is a key in functions
                tokens.push(variables[identifier_str]);
            } else {
                std::cout << "Name Error: undeclared variable/function: \"" << identifier_str << "\".\n";
                return 1;
            }
            break;
        case tok_number:
            tokens.push(num_val);
            break;
        default:
            std::cout << "Syntax Error: invalid character: \"" << char(token) << "\".\n";
            return 1;
        }

        token = gettok(src);
    }

    return 0;
}
