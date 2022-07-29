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
    SourceCode(const std::string &source) : source(source)
    {
        line_number = 1;
        column_number = 1;
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
        NUMBER,
        IDENTIFIER,
        MUL,
        DIV,
        ADD,
        SUB,
        LPAREN,
        RPAREN,
        T_UNKNOWN
    };
    static std::string getTypeName(Type type)
    {
        switch (type)
        {
        case NUMBER:
            return "NUMBER";
        case MUL:
            return "MUL";
        case DIV:
            return "DIV";
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case T_EOF:
            return "EOF";
        case LPAREN:
            return "LPAREN";
        case RPAREN:
            return "RPAREN";
        default:
            return "UNKNOWN";
        }
    }

    Type type;
    int value;      // for NUMBER
    std::string id; // for IDENTIFIER
    Token(Type type, int value = 0, const std::string &id = "")
        : type(type), value(value), id(id)
    {
    }
};

// Token stream override
std::ostream &operator<<(std::ostream &Str, Token const &token)
{
    Str << "Token(" << Token::getTypeName(token.type) << ": ";
    if (token.type == Token::NUMBER)
        Str << token.value;
    else if (token.type == Token::IDENTIFIER)
        Str << token.id;
    Str << ")";
    return Str;
}

// Lexer class
class Lexer
{
    char c;
    SourceCode source = SourceCode("");

public:
    Lexer(SourceCode &src)
    {
        source = src;
        c = ' ';
    }

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
            return Token(Token::NUMBER, val);
        }
        if (std::isalpha(c))
        {
            std::string id;
            while (std::isalnum(c))
                id += c, c = source.get_char();
            return Token(Token::IDENTIFIER, 0, id);
        }
        Token res = Token(Token::T_UNKNOWN);
        switch (c)
        {
        case '*':
            res = Token(Token::MUL);
            break;
        case '/':
            res = Token(Token::DIV);
            break;
        case '+':
            res = Token(Token::ADD);
            break;
        case '-':
            res = Token(Token::SUB);
            break;
        case '(':
            res = Token(Token::LPAREN);
            break;
        case ')':
            res = Token(Token::RPAREN);
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
    Token token;
    ASTNode(Token token) : token(token) {}
    virtual ~ASTNode() = default;
};

// Binary operator node class
class BinOpNode : public ASTNode
{
public:
    ASTNode *left;
    ASTNode *right;
    BinOpNode(const Token &token, ASTNode *left, ASTNode *right) : ASTNode(token), left(left), right(right) {}
};

// Number node class
class NumNode : public ASTNode
{
public:
    int value;
    NumNode(Token token) : ASTNode(token), value(token.value) {}
};

// Parser class
class Parser
{
    Lexer *lexer;
    Token current_token = Token(Token::T_UNKNOWN);

public:
    Parser(Lexer *lex)
    {
        lexer = lex;
        current_token = lexer->getNextToken();
    }

    // Compare current token type with the given token type and if they match then "eat"
    // the current token and assign the next token to current_token, otherwise raise an error.
    void eat(Token::Type type)
    {
        if (current_token.type == type)
        {
            current_token = lexer->getNextToken();
        }
        else
        {
            throw std::runtime_error("Syntax error: type " + Token::getTypeName(type) + " expected, but found" + Token::getTypeName(current_token.type));
        }
    }

    // factor : NUMBER | LPAREN expr RPAREN
    // TODO: factor : NUMBER | IDENTIFIER | LPAREN expr RPAREN
    ASTNode *factor()
    {
        if (current_token.type == Token::NUMBER)
        {
            Token t = current_token;
            eat(Token::NUMBER);
            return new NumNode(t);
        }
        else if (current_token.type == Token::LPAREN)
        {
            eat(Token::LPAREN);
            ASTNode *node = expr();
            eat(Token::RPAREN);
            return node;
        }
        else
        {
            throw std::runtime_error(
                "Syntax error" + lexer->getLocation() +
                ": NUMBER expected as factor, instead " + Token::getTypeName(current_token.type) + " was found");
        }
    }

    // term : factor  ((MUL | DIV) factor)*
    // EXAMPLE 1: term : factor MUL factor
    // EXAMPLE 2: term : factor MUL factor DIV factor
    // EXAMPLE 3: term : factor
    ASTNode *term()
    {
        ASTNode *left = factor();
        while (current_token.type == Token::MUL || current_token.type == Token::DIV)
        {
            Token t = current_token;
            if (t.type == Token::MUL)
                eat(Token::MUL);
            else if (t.type == Token::DIV)
                eat(Token::DIV);
            ASTNode *right = factor();
            left = new BinOpNode(t, left, right);
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
    ASTNode *expr()
    {
        ASTNode *left = term();

        while (current_token.type == Token::ADD || current_token.type == Token::SUB)
        {
            Token t = current_token;
            if (t.type == Token::ADD)
                eat(Token::ADD);
            else if (t.type == Token::SUB)
                eat(Token::SUB);
            ASTNode *right = term();
            left = new BinOpNode(t, left, right);
        }
        return left;
    }

    // parse
    ASTNode *parse()
    {
        return expr();
    }
};
// End Parser ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Interpreter +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class NodeVisitor
{
public:
    virtual ~NodeVisitor() = default;
    virtual int visit(ASTNode *node) = 0;
};

class Interpreter : public NodeVisitor
{
public:
    ASTNode *ast;
    Interpreter(ASTNode *root)
    {
        ast = root;
    }
    int interpret()
    {
        return visit(ast);
    }
    int visit(ASTNode *node)
    {
        switch (node->token.type)
        {
        case Token::NUMBER:
        {
            NumNode *num = dynamic_cast<NumNode *>(node);
            return num->value;
        }
        case Token::ADD:
        {
            BinOpNode *binop = dynamic_cast<BinOpNode *>(node);
            return visit(binop->left) + visit(binop->right);
        }
        case Token::SUB:
        {
            BinOpNode *binop = dynamic_cast<BinOpNode *>(node);
            return visit(binop->left) - visit(binop->right);
        }
        case Token::MUL:
        {
            BinOpNode *binop = dynamic_cast<BinOpNode *>(node);
            return visit(binop->left) * visit(binop->right);
        }
        case Token::DIV:
        {
            BinOpNode *binop = dynamic_cast<BinOpNode *>(node);
            return visit(binop->left) / visit(binop->right);
        }
        default:
            throw std::runtime_error("Error while interpreting AST: " + node->token.id + " expected");
        }
    }
};

// End Interpreter ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Driver code
int main()
{
    std::ifstream t("example.txt");
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    SourceCode source(str);
    Lexer *lex = new Lexer(source);
    // while (true)
    // {
    //     Token token = lex.getNextToken();
    //     if (token.type == Token::T_EOF)
    //         break;
    //     std::cout << token << std::endl;
    // }
    Parser par(lex);
    ASTNode *root = par.parse();

    Interpreter inter(root);
    std::cout << "Result: " << inter.interpret() << std::endl;
    return 0;
}