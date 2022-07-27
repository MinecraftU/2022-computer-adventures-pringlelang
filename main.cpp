#include <iostream>
#include <fstream>
#include <stack>
#include <streambuf>

struct SourceCode
{
    std::string raw;
    int idx = 0;

    SourceCode(std::string raw_in)
    {
        raw = raw_in;
    }

    char get_char()
    {
        if (idx == raw.length())
            return EOF;
        return raw[idx++];
    }
};

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

std::stack<int> tokens;

// gettok - Return the next token from standard input.
static int gettok(SourceCode &src)
{
    static int last_char = ' ';

    // Skip any whitespace.
    while (isspace(last_char))
        last_char = src.get_char();

    if (isalpha(last_char))
    { // identifier: [a-zA-Z][a-zA-Z0-9]*
        identifier_str = last_char;
        while (isalnum((last_char = src.get_char())))
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
        int args[2]; // REMINDER: convert to vector when 3+ args
        switch (token)
        {
        case tok_print:
            args[0] = tokens.top();
            tokens.pop();
            std::cout << args[0];
            break;
        case '+':
            args[1] = tokens.top();
            tokens.pop();
            args[0] = tokens.top();
            tokens.pop();
            tokens.push(args[0] + args[1]);
            break;
        case '*':
            args[1] = tokens.top();
            tokens.pop();
            args[0] = tokens.top();
            tokens.pop();
            tokens.push(args[0] * args[1]);
            break;
        case '-':
            args[1] = tokens.top();
            tokens.pop();
            args[0] = tokens.top();
            tokens.pop();
            tokens.push(args[0] - args[1]);
            break;
        case '/':
            args[1] = tokens.top();
            tokens.pop();
            args[0] = tokens.top();
            tokens.pop();
            tokens.push(args[0] / args[1]);
            break;
        // case tok_identifier:
        //     break;
        case tok_number:
            tokens.push(num_val);
            break;
        default:
            std::cout << "Something went wrong!\n";
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
