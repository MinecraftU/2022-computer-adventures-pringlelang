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
        T_MUL,
        T_DIV,
        T_ADD,
        T_SUB,
        T_UNKNOWN
    };

    Type type;
    int value;      // for T_NUMBER
    char op;        // for T_MUL, T_DIV, T_ADD, T_SUBT_OPERATOR
    std::string id; // for T_IDENTIFIER
    Token(Type type, int value = 0, const std::string &id = "")
        : type(type), value(value), id(id)
    {
        switch (type)
        {
        case T_MUL:
            op = '*';
            break;
        case T_DIV:
            op = '/';
            break;
        case T_ADD:
            op = '+';
            break;
        case T_SUB:
            op = '-';
            break;
        default:
            break;
        }
    }
};

// Token stream override
std::ostream &operator<<(std::ostream &Str, Token const &token)
{
    // print something from v to str, e.g: Str << v.getX();
    switch (token.type)
    {
    case Token::T_NUMBER:
        Str << std::to_string(token.value);
        break;
    case Token::T_IDENTIFIER:
        Str << token.id;
        break;
    case Token::T_MUL:
        Str << "*";
        break;
    case Token::T_DIV:
        Str << "/";
        break;
    case Token::T_ADD:
        Str << "+";
        break;
    case Token::T_SUB:
        Str << "-";
        break;
    default:
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
    Lexer(SourceCode &src) : source(src)
    {
        c = source.get_char();
    };

    Token getNextToken()
    {
        while (isspace(c)) // skip whitespace
        {
            c = source.get_char();
        }

        if (c == -1)
            return Token(Token::T_EOF);

        if (std::isdigit(c))
        {
            int val = c - '0';
            c = source.get_char();
            while (std::isdigit(c))
            {
                val = val * 10 + c - '0';
                c = source.get_char();
            }
            return Token(Token::T_NUMBER, val);
        }
        if (std::isalpha(c))
        {
            std::string id;
            while (std::isalnum(c))
                id += c, c = source.get_char();
            return Token(Token::T_IDENTIFIER, 0, id);
        }
        Token res = Token(Token::T_UNKNOWN);
        switch (c)
        {
        case '*':
            res = Token(Token::T_MUL);
            break;
        case '/':
            res = Token(Token::T_DIV);
            break;
        case '+':
            res = Token(Token::T_ADD);
            break;
        case '-':
            res = Token(Token::T_SUB);
            break;
        default:
            break;
        }
        c = source.get_char();
        return res;
    }

    std::string getLocation()
    {
        return "[Ln " + std::to_string(source.get_line_number()) + ", Col" + std::to_string(source.get_column_number()) + "]";
    }

    ~Lexer() = default;
};

// Parser ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// AST node class
class ASTNode
{
public:
    virtual ~ASTNode() = default;
};

// Binary operator node class
class BinOpNode : public ASTNode
{
public:
    ASTNode left;
    ASTNode right;
    Token token;
    char op;
    BinOpNode(ASTNode left, ASTNode right, const Token &token) : left(left), right(right), token(token) { op = token.op; }
};

// Number node class
class NumNode : public ASTNode
{
public:
    Token token;
    int value;
    NumNode(Token token) : token(token) { value = token.value; }
};

// Parser class
class Parser
{
public:
    Lexer lexer;
    Token current_token = Token(Token::T_UNKNOWN);
    Parser(Lexer &lexer) : lexer(lexer) { current_token = lexer.getNextToken(); };

    // Compare current token type with the given token type and if they match then "eat"
    // the current token and assign the next token to current_token, otherwise raise an error.
    void eat(Token::Type type)
    {
        if (current_token.type == type)
            current_token = lexer.getNextToken();
        else
            throw std::runtime_error("Syntax error: " + current_token.id + " expected");
    }

    // factor : NUMBER
    // TODO: factor : NUMBER | IDENTIFIER | '(' expr ')'
    ASTNode factor()
    {
        Token t = current_token;
        if (t.type == Token::T_NUMBER)
        {
            eat(Token::T_NUMBER);
            return NumNode(t);
        }
        else
        {
            throw std::runtime_error("Syntax error" + lexer.getLocation() + ": NUMBER expected as factor");
        }
    }

    // term : factor  ((MUL | DIV) factor)*
    // EXAMPLE 1: term : factor MUL factor
    // EXAMPLE 2: term : factor MUL factor DIV factor
    // EXAMPLE 3: term : factor
    ASTNode term()
    {
        ASTNode left = factor();
        while (current_token.op == '*' || current_token.op == '/')
        {
            Token t = current_token;
            if (t.op == '*')
                eat(Token::T_MUL);
            else if (t.op == '/')
                eat(Token::T_DIV);
            ASTNode right = factor();
            left = BinOpNode(left, right, t);
        }
        return left;
    }

    // expr : term ((ADD | SUB) term)*
    // EXAMPLE 1: expr : term ADD term
    // EXAMPLE 1 with more details: expr : factor MUL factor ADD factor DIV factor
    // EXAMPLE 1 with even more details: expr : 1 MUL 2 ADD 4 DIV 2
    //                                          (1 * 2) + (4 / 2) = 4
    // EXAMPLE 2: expr : term ADD term SUB term
    // EXAMPLE 2 with more details: expr : factor MUL factor ADD factor DIV factor SUB factor
    // EXAMPLE 2 with even more details: expr : 1 MUL 2 ADD 4 DIV 2 SUB 1
    //                                          1 * 2 + 4 / 2 - 1
    //                                          ((1 * 2) + (4 / 2)) - 1 = 3
    ASTNode expr()
    {
        ASTNode left = term();

        while (current_token.op == '+' || current_token.op == '-')
        {
            Token t = current_token;
            if (t.op == '+')
                eat(Token::T_ADD);
            else if (t.op == '-')
                eat(Token::T_SUB);
            ASTNode right = term();
            left = BinOpNode(left, right, t);
        }
        return left;
    }

    // parse
    ASTNode parse()
    {
        return expr();
    }
};
// End Parser ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//
// Driver code
int main()
{
    SourceCode source("example.txt");
    Lexer lex(source);
    // std::cout << lex.getNextToken() << std::endl;
    Token t = Token(Token::T_UNKNOWN);
    do
    {
        t = lex.getNextToken();
        std::cout << t << std::endl;
    } while (t.type != Token::T_EOF);

    // Parser par(lex);
    // ASTNode root = par.parse();
    // std::cout << root << std::endl;
    return 0;
}