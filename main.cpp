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

    // primary
    tok_identifier = -4,
    tok_number = -5
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number
static FILE* pFile = nullptr;

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

/** ===----------------------------------------------------------------------===
 Abstract Syntax Tree (aka Parse Tree)
===----------------------------------------------------------------------===**/

namespace
{
    /// AST_Expr - Base class for all expression nodes.
    class AST_Expr
    {
    public:
        virtual ~AST_Expr() = default;
    };

    /// NumberAST_Expr - Expression class for numeric literals like "1.0".
    class NumberAST_Expr : public AST_Expr
    {
        double val;

    public:
        NumberAST_Expr(double val) : val(val) {}
    };

    /// VariableAST_Expr - Expression class for referencing a variable, like "a".
    class VariableAST_Expr : public AST_Expr
    {
        std::string name;

    public:
        VariableAST_Expr(const std::string &name) : name(name) {}
    };

    /// BinaryAST_Expr - Expression class for a binary operator.
    class BinaryAST_Expr : public AST_Expr
    {
        char Op;
        std::unique_ptr<AST_Expr> left, right;

    public:
        BinaryAST_Expr(char Op, std::unique_ptr<AST_Expr> left,
                       std::unique_ptr<AST_Expr> right)
            : Op(Op), left(std::move(left)), right(std::move(right)) {}
    };

    /// CallAST_Expr - Expression class for function calls.
    class CallAST_Expr : public AST_Expr
    {
        std::string callee;
        std::vector<std::unique_ptr<AST_Expr>> args;

    public:
        CallAST_Expr(const std::string &callee,
                     std::vector<std::unique_ptr<AST_Expr>> args)
            : callee(callee), args(std::move(args)) {}
    };

    /// PrototypeAST - This class represents the "prototype" for a function,
    /// which captures its name, and its argument names (thus implicitly the number
    /// of arguments the function takes).
    class PrototypeAST
    {
        std::string name;
        std::vector<std::string> args;

    public:
        PrototypeAST(const std::string &name, std::vector<std::string> args)
            : name(name), args(std::move(args)) {}

        const std::string &getName() const { return name; }
    };

    /// FunctionAST - This class represents a function definition itself.
    class FunctionAST
    {
        std::unique_ptr<PrototypeAST> proto;
        std::unique_ptr<AST_Expr> body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> proto,
                    std::unique_ptr<AST_Expr> body)
            : proto(std::move(proto)), body(std::move(body)) {}
    };

} // end anonymous namespace

//
//===----------------------------------------------------------------------===//
// Parser XXX
//===----------------------------------------------------------------------===//

/// CurTok - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  
static int CurTok;
/// getNextToken reads another token from the lexer and updates 
/// CurTok with its results.
static int getNextToken() { return CurTok = gettok(); }

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
static std::map<char, int> BinopPrecedence;

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence()
{
    if (!isascii(CurTok))
        return -1;

    // Make sure it's a declared binop.
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0)
        return -1;
    return TokPrec;
}

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<AST_Expr> LogError(const char *Str)
{
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str)
{
    LogError(Str);
    return nullptr;
}

static std::unique_ptr<AST_Expr> ParseExpression();

/// numberexpr ::= number
static std::unique_ptr<AST_Expr> ParseNumberExpr()
{
    auto Result = std::make_unique<NumberAST_Expr>(NumVal);
    getNextToken(); // consume the number
    return std::move(Result);
}

/// parenexpr ::= '(' expression ')'
static std::unique_ptr<AST_Expr> ParseParenExpr()
{
    getNextToken(); // eat (.
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogError("expected ')'");
    getNextToken(); // eat ).
    return V;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static std::unique_ptr<AST_Expr> ParseIdentifierExpr()
{
    std::string IdName = IdentifierStr;

    getNextToken(); // eat identifier.

    if (CurTok != '(') // Simple variable ref.
        return std::make_unique<VariableAST_Expr>(IdName);

    // Call.
    getNextToken(); // eat (
    std::vector<std::unique_ptr<AST_Expr>> Args;
    if (CurTok != ')')
    {
        while (true)
        {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    // Eat the ')'.
    getNextToken();

    return std::make_unique<CallAST_Expr>(IdName, std::move(Args));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
static std::unique_ptr<AST_Expr> ParsePrimary()
{
    switch (CurTok)
    {
    default:
        return LogError("unknown token when expecting an expression");
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_number:
        return ParseNumberExpr();
    case '(':
        return ParseParenExpr();
    }
}

/// binoRIGHTp
///   ::= ('+' primary)*
static std::unique_ptr<AST_Expr> ParseBinOpRIGHT(int ExprPrec,
                                               std::unique_ptr<AST_Expr> LEFT)
{
    // If this is a binop, find its precedence.
    while (true)
    {
        int TokPrec = GetTokPrecedence();

        // If this is a binop that binds at least as tightly as the current binop,
        // consume it, otherwise we are done.
        if (TokPrec < ExprPrec)
            return LEFT;

        // Okay, we know this is a binop.
        int BinOp = CurTok;
        getNextToken(); // eat binop

        // Parse the primary expression after the binary operator.
        auto RIGHT = ParsePrimary();
        if (!RIGHT)
            return nullptr;

        // If BinOp binds less tightly with RIGHT than the operator after RIGHT, let
        // the pending operator take RIGHT as its LEFT.
        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec)
        {
            RIGHT = ParseBinOpRIGHT(TokPrec + 1, std::move(RIGHT));
            if (!RIGHT)
                return nullptr;
        }

        // Merge LEFT/RIGHT.
        LEFT =
            std::make_unique<BinaryAST_Expr>(BinOp, std::move(LEFT), std::move(RIGHT));
    }
}

/// expression
///   ::= primary binoRIGHTp
///
static std::unique_ptr<AST_Expr> ParseExpression()
{
    auto LEFT = ParsePrimary();
    if (!LEFT)
        return nullptr;

    return ParseBinOpRIGHT(0, std::move(LEFT));
}

/// prototype
///   ::= id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype()
{
    if (CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    // success.
    getNextToken(); // eat ')'.

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

/// definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition()
{
    getNextToken(); // eat def.
    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;

    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

/// toplevelexpr ::= expression
static std::unique_ptr<FunctionAST> ParseTopLevelExpr()
{
    if (auto E = ParseExpression())
    {
        // Make an anonymous proto.
        auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
                                                    std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

/// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern()
{
    getNextToken(); // eat extern.
    return ParsePrototype();
}

//===----------------------------------------------------------------------===//
// Top-Level parsing XXX
//===----------------------------------------------------------------------===//

static void HandleDef()
{
    if (ParseDefinition())
    {
        fprintf(stderr, "Parsed a function definition.\n");
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleExtern()
{
    if (ParseExtern())
    {
        fprintf(stderr, "Parsed an extern\n");
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleTopLevelExpression()
{
    // Evaluate a top-level expression into an anonymous function.
    if (ParseTopLevelExpr())
    {
        fprintf(stderr, "Parsed a top-level expr\n");
    }
    else
    {
        // Skip token for error recovery.
        getNextToken();
    }
}

/// top ::= definition | external | expression | ';'
static void MainLoop()
{
    while (true)
    {
        switch (CurTok)
        {
        case tok_eof:
            return;
        case ';': // ignore top-level semicolons.
            getNextToken();
            break;
        case tok_def:
            HandleDef();
            break;
        case tok_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
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
