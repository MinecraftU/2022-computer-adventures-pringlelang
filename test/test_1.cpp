#include <catch.hpp>

#include "interpreter.hpp"

using namespace std;

SCENARIO( "file input test", "[test_1.cpp]" )
{
    ifstream t("../../test/test_1_input.txt");
    if (!t.is_open()) throw runtime_error("File not found in specified location");
    
    SourceCode source(t);
    Lexer *lex = new Lexer(source);
    Parser par(lex);
    ASTNode *root = par.parse();
    Interpreter myInterp(root);
    SourceCode src = SourceCode(t);

    REQUIRE( myInterp.interpret() == 2 );
}

SCENARIO( "multiply"){
    string src = "2 * 5";
    SourceCode source(src);
    Lexer *lex = new Lexer(source);
    Parser par(lex);
    ASTNode *root = par.parse();
    Interpreter myInterp(root);
    REQUIRE( myInterp.interpret() == 10 );
}

SCENARIO( "divide"){
    string src = "6 / 2";
    SourceCode source(src);
    Lexer *lex = new Lexer(source);
    Parser par(lex);
    ASTNode *root = par.parse();
    Interpreter myInterp(root);
    REQUIRE( myInterp.interpret() == 3 );
}

SCENARIO( "subtract"){
    string src = "6 - 2";
    SourceCode source(src);
    Lexer *lex = new Lexer(source);
    Parser par(lex);
    ASTNode *root = par.parse();
    Interpreter myInterp(root);
    REQUIRE( myInterp.interpret() == 4 );
}

// I have no idea what the second argument here does, apparently its not used
SCENARIO( "negative numbers"){
    string src = "-6 - 2";
    SourceCode source(src);
    Lexer *lex = new Lexer(source);
    Parser par(lex);
    ASTNode *root = par.parse();
    Interpreter myInterp(root);
    REQUIRE( myInterp.interpret() == -8 );
}

// SCENARIO( "divide by zero", "[test_1.cpp]"){
//     string src = "6 / 0";
//     SourceCode source(src);
//     Lexer *lex = new Lexer(source);
//     Parser par(lex);
//     ASTNode *root = par.parse();
//     Interpreter myInterp(root);
//     REQUIRE_THROWS( myInterp.interpret() == 6);
// }