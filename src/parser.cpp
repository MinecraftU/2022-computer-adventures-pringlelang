#include "parser.hpp"

int Parser::gettok(SourceCode &src)
{
    int last_char = ' ';

    // Skip any whitespace.
    while (isspace(last_char))
    {
        last_char = src.get_char();
    }

    if (isalpha(last_char)) // function names can currently be anything; make it so it can only be alphanumeric
    {                       // identifier: [a-zA-Z][a-zA-Z0-9]*
        identifier_str = last_char;
        while (isalnum((last_char = src.get_char())))
        {
            identifier_str += last_char;
        }
        auto tok = command_to_token.find(identifier_str);
        if (tok != command_to_token.end())
            return tok->second;
        return tok_identifier;
    }

    if (isdigit(last_char))
    { // Number: [0-9]+
        std::string NumStr;
        do
        {
            NumStr += last_char;
            last_char = src.get_char();
        } while (isdigit(last_char));

        num_val = std::stoi(NumStr);
        return tok_number;
    }

    if (last_char == '"') {
        std::string str;
        last_char = src.get_char();
        do
        {
            str += last_char;
            last_char = src.get_char();
        } while (last_char != '"');

        str_val = str;
        return tok_string;
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
        Value x, y, z;
        // for functions/operators
        std::vector<Value> args;
        // for case tok_func
        char c = ' ';
        int b_count = 1; // unmatched bracket pair count
        std::string name = "";
        std::vector<std::string> arg_names;
        std::string arg_name = "";
        std::string inside_src = "";
        // for case tok_loop
        SourceCode new_src;
        // TODO: throw if size of stack not ok for token type (unary op, binary op, etc) - ignore if function
        switch (token)
        {
        case tok_pop:
            if (stack.empty())
                throw std::logic_error("Cannot pop empty stack");
            stack.pop();
            break;
        case tok_dup:
            stack.push(stack.top());
            break;
        case tok_twodup:
            x = stack.top();
            stack.pop();
            y = stack.top();
            stack.push(x);
            stack.push(y);
            stack.push(x);
            break;
        case tok_swap:
            x = stack.top();
            stack.pop();
            y = stack.top();
            stack.pop();
            stack.push(x);
            stack.push(y);
            break;
        case tok_over:
            x = stack.top();
            stack.pop();
            y = stack.top();
            stack.pop();
            z = stack.top();
            stack.push(y);
            stack.push(x);
            stack.push(z);
            break;
        case tok_print:
            args.push_back(stack.top());
            stack.pop();
            std::cout << args[0];
            break;
        case tok_func:
            c = src.get_char();
            while (c != '{')
            {
                if (c == ' ' && arg_name != "")
                {
                    if (name == "")
                    {
                        name = arg_name;
                    }
                    else
                    {
                        arg_names.push_back(arg_name);
                    }
                    arg_name = "";
                }
                else
                {
                    arg_name += c;
                }
                c = src.get_char();
            }

            while (b_count != 0)
            {
                c = src.get_char();

                inside_src += c;
                if (c == '{')
                    b_count++;
                if (c == '}')
                    b_count--;
            }
            inside_src.pop_back();
            functions[name] = std::move(SourceCode(inside_src, arg_names));
            break;
        case tok_var:
            if (gettok(src) != tok_identifier)
            {
                std::cout << "Name Error: invalid identifier name.\n";
                return 1;
            }
            args.push_back(stack.top());
            stack.pop();
            variables[identifier_str] = args[0];
            break;
        case tok_loop:
            c = src.get_char();
            while (b_count != 0)
            {
                c = src.get_char();

                inside_src += c;
                if (c == '{')
                    b_count++;
                if (c == '}')
                    b_count--;
            }
            inside_src.pop_back();

            new_src = std::move(SourceCode(inside_src));
            while (true)
            {
                if (parse(new_src) != 0)
                    break;

                new_src.reset_idx();
            }
            break;
        case tok_break:
            return 2;
            break;
        // TODO: refactor this for performance by jumping to closing bracket on false
        //  instead of expensive recursive call to parse
        case tok_if:
            c = src.get_char();
            while (b_count != 0)
            {
                c = src.get_char();

                inside_src += c;
                if (c == '{')
                    b_count++;
                if (c == '}')
                    b_count--;
            }
            inside_src.pop_back();
            args.push_back(stack.top());
            stack.pop();
            if (args[0].get_int() > 0)
            {
                new_src = SourceCode(inside_src);
                if (parse(new_src) != 0)
                    return 2;
            }
            break;
        case tok_identifier:
        {
            auto tok_func_id = functions.find(identifier_str);
            auto tok_var_id = variables.find(identifier_str);
            if (tok_func_id != functions.end())
            { // if identifier_str is a key in functions
                tok_func_id->second.reset_idx();

                std::vector<std::string> str_args;
                size_t s = tok_func_id->second.get_arg_names().size();
                for (size_t i = 0; i < s; i++)
                {
                    str_args.push_back(tok_func_id->second.get_arg_names()[s - i - 1]);
                    variables[tok_func_id->second.get_arg_names()[s - i - 1]] = stack.top();
                    stack.pop();
                }

                parse(tok_func_id->second);

                for (std::string str_arg : str_args) {
                    variables.erase(variables.find(str_arg));
                }
            }
            else if (tok_var_id != variables.end())
            { // if identifier_str is a key in functions
                stack.push(tok_var_id->second);
            }
            else
            {
                std::cout << "Name Error: undeclared variable/function: \"" << identifier_str << "\".\n";
                return 1;
            }
        }
        break;
        case tok_number:
            stack.push(num_val);
            break;
        case tok_string:
            stack.push(str_val);
            break;
        default: // operator or unrecognized character
            if (operators.find(token) == operators.end())
            {
                std::cout << "Syntax Error: unrecognized character: \"" << char(token) << "\".\n";
                return 1;
            }
            if (stack.size() < 2 && token != '!')
            {
                std::cout << "Error: not enough operands in stack.\n";
                return 1;
            }
            switch (token)
            {
            case '.':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                stack.push(Value(args[1].get_string()[args[0].get_int()]));
                break;
            case '+':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                if (args[1].get_type() == type_int && args[0].get_type() == type_int)
                {
                    stack.push(Value(args[1].get_int() + args[0].get_int()));
                }
                else if (args[1].get_type() == type_string && args[0].get_type() == type_string)
                {
                    stack.push(Value(args[1].get_string() + args[0].get_string()));
                }
                else
                {
                    std::cout << "Argument Error: incorrect argument types (did not get two ints or two strings)!";
                }
                break;
            case '*':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                stack.push(Value(args[1].get_int() * args[0].get_int()));
                break;
            case '-':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                stack.push(Value(args[1].get_int() - args[0].get_int()));
                break;
            case '/':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                stack.push(Value(args[1].get_int() / args[0].get_int()));
                break;
            case '%':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                stack.push(Value(args[1].get_int() % args[0].get_int()));
                break;
            case '^':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                stack.push(Value((int)pow(args[1].get_int(), args[0].get_int()))); // coerce double to int by flooring
                break;
            case '<':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                stack.push(Value(args[1].get_int() < args[0].get_int()));
                break;
            case '>':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                stack.push(Value(args[1].get_int() > args[0].get_int()));
                break;
            case '=':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                stack.push(Value(args[1].get_int() == args[0].get_int()));
                break;
            case '&':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                stack.push(Value(args[1].get_int() && args[0].get_int()));
                break;
            case '|':
                args.push_back(stack.top());
                stack.pop();
                args.push_back(stack.top());
                stack.pop();
                stack.push(Value(args[1].get_int() || args[0].get_int()));
                break;
            case '!':
                if (stack.empty())
                {
                    std::cout << "Error: no operand in stack.\n";
                    return 1;
                }
                stack.top() = !stack.top().get_int();
                break;
            }
        }
        token = gettok(src);
    }
    return 0;
}
