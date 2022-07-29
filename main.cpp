#include <algorithm>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <map>
#include <stack>
#include <vector>

struct SourceCode
{
    std::string raw;
    int idx = 0;
    std::vector<std::string> arg_names;

    SourceCode(std::string raw_in)
    {
        raw = raw_in;
    }

    SourceCode(std::string raw_in, std::vector<std::string> arg_names_in)
    {
        raw = raw_in;
        arg_names = arg_names_in;
    }

    // for some reason a constructor with no arguments gets called, and this is working fine :/
    SourceCode()
    {
    }

    char get_char()
    {
        if (idx == raw.length())
            return EOF;
        return raw[idx++];
    }

    void reset_idx()
    {
        idx = 0;
    }

    std::string replace_args(std::vector<std::string> args) {
        if (args.size() != arg_names.size()) {
            std::cout << "Argument Error: Incorrect amount of aruments (expected " << arg_names.size() << " arguments, got " << args.size() << " arguments).";
        }

        std::string ret = raw;
        for (size_t i = 0; i < args.size(); i++) {
            // https://stackoverflow.com/a/3418285/16191068
            if (arg_names[i].empty()) continue;
            size_t start_pos = 0;
            while ((start_pos = ret.find(arg_names[i], start_pos)) != std::string::npos) {
                ret.replace(start_pos, arg_names[i].length(), args[i]);
                start_pos += args[i].length();
            }
        }

        return ret;
    }
};

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
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
static std::string identifier_str; // Filled in if tok_identifier
static double num_val;             // Filled in if tok_number

std::stack<int> tokens;
std::map<std::string, SourceCode> functions;
std::map<std::string, int> variables;

// gettok - Return the next token from standard input.
static int gettok(SourceCode &src)
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
        return tok_identifier;
    }

    if (isdigit(last_char) || last_char == '.')
    { // Number: [0-9.]+
        std::string NumStr;
        do
        {
            NumStr += last_char;
            last_char = src.get_char();
        } while (isdigit(last_char) || last_char == '.');

        num_val = strtod(NumStr.c_str(), 0);
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

int parse(SourceCode &src)
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
            functions[name] = SourceCode(inside_src, arg_names);
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
                for (size_t i = 0; i < functions[identifier_str].arg_names.size(); i++) {
                    str_args.push_back(std::to_string(tokens.top()));
                    tokens.pop();
                }
                std::reverse(str_args.begin(), str_args.end());
                SourceCode replaced = SourceCode(functions[identifier_str].replace_args(str_args));
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

int main()
{
    std::ifstream t("example.txt");

    std::string raw_src((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
    SourceCode src = SourceCode(raw_src);

    parse(src);

    t.close();
}
