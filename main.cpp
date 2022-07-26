#include <iostream>
#include <fstream>
#include <stack>

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token
{
    tok_eof = -1,

    // commands
    tok_print = -2,

    // primary
    tok_identifier = -3,
    tok_number = -4,
};
static std::string identifier_str; // Filled in if tok_identifier
static double num_val;             // Filled in if tok_number

// gettok - Return the next token from standard input.
static int gettok(FILE *input_file)
{
    static int last_char = ' ';

    // Skip any whitespace.
    while (isspace(last_char))
        last_char = getc(input_file);

    if (isalpha(last_char))
    { // identifier: [a-zA-Z][a-zA-Z0-9]*
        identifier_str = last_char;
        while (isalnum((last_char = getc(input_file))))
            identifier_str += last_char;

        if (identifier_str == "print")
            return tok_print;
        return tok_identifier;
    }

    if (isdigit(last_char) || last_char == '.')
    { // Number: [0-9.]+
        std::string NumStr;
        do
        {
            NumStr += last_char;
            last_char = getc(input_file);
        } while (isdigit(last_char) || last_char == '.');

        num_val = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if (last_char == '#')
    {
        // Comment until end of line.
        do
            last_char = getc(input_file);
        while (last_char != EOF && last_char != '\n' && last_char != '\r');

        if (last_char != EOF)
            return gettok(input_file);
    }

    // Check for end of file.  Don't eat the EOF.
    if (last_char == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int this_char = last_char;
    last_char = getc(input_file);
    return this_char;
}

int main()
{
    FILE *input_file = fopen("example.txt", "r");

    std::stack<int> tokens;
    int token = gettok(input_file);
    while (token != -1)
    {
        int args[2]; // REMINDER: convert to vector when 3+ args
        switch (token)
        {
        case tok_print:
            args[0] = tokens.top();
            tokens.pop();
            std::cout << args[0];
            break;
        case '+':
            args[0] = tokens.top();
            tokens.pop();
            args[1] = tokens.top();
            tokens.pop();
            tokens.push(args[0] + args[1]);
            break;
        case '*':
            args[0] = tokens.top();
            tokens.pop();
            args[1] = tokens.top();
            tokens.pop();
            tokens.push(args[0] * args[1]);
            break;
        //case tok_identifier:
        //    break;
        case tok_number:
            tokens.push(num_val);
            break;
        default:
            std::cout << "Something went wrong!\n";
            return 1;
        }

        token = gettok(input_file);
    }

    fclose(input_file);
}
