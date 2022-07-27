#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <streambuf>
#include <fstream>

// Source code class
class SourceCode
{
    int line_number;
    int column_number;
    int idx = -1;
    std::string source;

public:
    SourceCode(const std::string &filename)
    {
        std::ifstream t(filename);
        std::string str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        source = str;
        line_number = 0;
        column_number = 0;
    }

    char get_char()
    {
        if (idx == source.size())
            return -1;
        char c = source[++idx];
        if (c == '\n')
        {
            line_number++;
            column_number = 0;
        }
        else
        {
            column_number++;
        }
        return c;
    }

    int get_line_number()
    {
        return line_number;
    }
    int get_column_number()
    {
        return column_number;
    }

    ~SourceCode() = default;

private:
    std::string filename_;
    std::vector<std::string> lines_;
};



// Token class
class Token
{
public:
    enum Type
    {
        T_EOF,
        T_NUMBER,
        T_IDENTIFIER,
        T_OPERATOR
    };

    Type type;
    int value;   // for T_NUMBER
    char op;        // for T_OPERATOR
    std::string id; // for T_IDENTIFIER
    Token(Type type, int value = 0, char op = 0, const std::string &id = "")
        : type(type), value(value), op(op), id(id) {}
};

// Token stream override
std::ostream &operator<<(std::ostream &Str, Token const &token)
{
    // print something from v to str, e.g: Str << v.getX();
    switch (token.type)
    {
    case Token::T_EOF:
        Str << "<EOF>";
        break;
    case Token::T_NUMBER:
        Str << std::to_string(token.value);
        break;
    case Token::T_IDENTIFIER:
        Str << token.id;
        break;
    case Token::T_OPERATOR:
        Str << std::string(1, token.op);
        break;
    default:
        Str << "<Unknown Symbol>: ";
        break;
    }
    return Str;
}



// Lexer class
class Lexer
{
    SourceCode source;
    char c;

public:
    Lexer(SourceCode &src) : source(src){
        c = source.get_char();
    };

    Token getNextToken()
    {
        if (c == -1)
            return Token(Token::T_EOF, 0);

        while (std::isspace(c)) // skip whitespace
        {
            c = source.get_char();
        }
        
        if (std::isdigit(c))
        {
            int val = c - '0';
            while (std::isdigit(c = source.get_char()))
                val = val * 10 + c - '0';
            return Token(Token::T_NUMBER, val);
        }
        if (std::isalpha(c))
        {
            std::string id;
            while (std::isalnum(c))
                id += c, c = source.get_char();
            return Token(Token::T_IDENTIFIER, 0, 0, id);
        }
        return Token(Token::T_OPERATOR, 0, c);
    }

    
    ~Lexer() = default;
};



// Parser

// Driver code
int main()
{
    SourceCode source("example.txt");
    Lexer lexer(source);
    std::cout << lexer.getNextToken() << std::endl;
    return 0;
}