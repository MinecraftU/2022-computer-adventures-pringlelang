#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

//===----------------------------------------------------------------------===//
// Lexer a.k.a. scanner a.k.a. lexical analysis XXX
//===----------------------------------------------------------------------===//

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token
{
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,
    tok_print = -4,

    // primary
    tok_identifier = -5,
    tok_number = -6,

    // control
    tok_if = -7,
    tok_else = -8
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number
static FILE *pFile = nullptr;

// gettok - Return the next token from standard input.
static int gettok()
{
    static int LastChar = ' ';

    // Skip any whitespace.
    while (isspace(LastChar))
        LastChar = getc(pFile);

    if (isalpha(LastChar))
    { // identifier: [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getc(pFile))))
            IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return tok_def;
        if (IdentifierStr == "extern")
            return tok_extern;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.')
    { // Number: [0-9.]+
        std::string NumStr;
        do
        {
            NumStr += LastChar;
            LastChar = getc(pFile);
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    if (LastChar == '#')
    {
        // Comment until end of line.
        do
            LastChar = getc(pFile);
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getc(pFile);
    return ThisChar;
}

/// CurTok - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.
static int CurTok;
/// getNextToken reads another token from the lexer and updates
/// CurTok with its results.
static int getNextToken() { return CurTok = gettok(); }

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
static std::map<char, int> BinopPrecedence;
static std::map<std::string, Node> IdentifierMap;

//===----------------------------------------------------------------------===//
// Tree Maker. XXX
//===----------------------------------------------------------------------===//

class Node {
public:
    virtual ~Node()  = default;
    
    virtual double evaluate() const = 0;
};

class NumberNode : public Node {
public:
    double val;
    NumberNode(double val) : val(val) {}
    double evaluate() const override { return val; }
};

class IdentifierNode : public Node {
public:
    std::string name;
    IdentifierNode(std::string name) : name(name) {}
    double evaluate() const override {
        if (IdentifierMap.count(name) == 0)
            return 0;
        return IdentifierMap.at(name)->evaluate();
    }
};

class BinOpNode : public Node
{
  public:
    int token;
    BinOpNode(int tok, Node *lhs, Node *rhs) : token(tok), LHS(lhs), RHS(rhs) {}
    Node *LHS, *RHS;
    double evaluate() const override {
        switch (token) {
        case '+':
            return LHS->evaluate() + RHS->evaluate();
        case '-':
            return LHS->evaluate() - RHS->evaluate();
        case '*':
            return LHS->evaluate() * RHS->evaluate();
        case '/':
            return LHS->evaluate() / RHS->evaluate();
        }
    }
};

/// top ::= definition | external | expression | ';'
static void MainLoop()
{
    Node* root = nullptr;
    Node* cur = root;
    while (true)
    {
        switch (CurTok)
        {
        case tok_eof:
            return;
        case ';': // ignore top-level semicolons.
            getNextToken();
            break;
        case tok_number:
            cur = new NumberNode(NumVal);
            getNextToken();
            break;
        case '+':
            cur = new BinOpNode(CurTok, cur, nullptr);
            getNextToken();
            break;
        default:
            break;
        }
    }
}

//===----------------------------------------------------------------------===//
// Main driver code. XXX
//===----------------------------------------------------------------------===//

int main()
{
    // Install standard binary operators.
    // 1 is lowest precedence.
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40; // highest.

    // Open the input file.
    pFile = fopen("example.txt", "r");
    // Prime the first token.
    getNextToken();

    // Run the main "interpreter loop" now.
    MainLoop();

    return 0;
}
