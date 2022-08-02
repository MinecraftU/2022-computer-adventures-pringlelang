#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "source_code.hpp"
#include "parser.hpp"

// helper function so i don't have to type all this out to check top of stack after parsing
int get_top(std::string raw_src) { 
    Parser parser;
    SourceCode src = SourceCode(raw_src);
    parser.parse(src);

    return parser.get_stack().top();
}

int get_exit_code(std::string raw_src) {
    Parser parser;
    SourceCode src = SourceCode(raw_src);
    
    return parser.parse(src);
}

TEST_CASE("Addition is computed", "[addition]") {
    int top = get_top(
        "3 4 +"
    );
    REQUIRE(top == 7);
}

TEST_CASE("Uninitialized identifier throws error", "[name error]") {
    int exit_code = get_exit_code(
        "3 4 exp"
    );
    REQUIRE(exit_code == 1);
}

TEST_CASE("Invalid syntax throws error", "[syntax error]") {
    int exit_code = get_exit_code(
        "^@#$%@&# #^T&D FTG FGYE*@#& DFSJHK I LIKE TRAINS!!!!!!!"
    );
    REQUIRE(exit_code == 1);
}

TEST_CASE("Top of stack printed to stdout", "[output]") {
    int exit_code = get_exit_code(
        "15 print"
    );
    REQUIRE(exit_code == 0);
}

TEST_CASE("Math expression is computed", "[math expression]") {
    int top = get_top(
        "3 5 * 4 + 6 3 / 6 3 / + * 3 7 * -"
    );
    REQUIRE(top == 55);
}

TEST_CASE("Function is delcared and called", "[function]") {
    int exit_code = get_exit_code(
        "FUNC {printSeven} {} {"
        "    7 print"
        "}"
        "printSeven"
    );
    REQUIRE(exit_code == 0);
}

TEST_CASE("Function with return value is computed", "[function return]") {
    int top = get_top(
        "FUNC {seven} {} {"
        "    7"
        "}"
        "seven"
    );
    REQUIRE(top == 7);
}

TEST_CASE("Function with return value and arguments is computed", "[function arguments]") {
    int top = get_top(
        "FUNC {sum4} {a b c d} {"
        "    a b + c + d +"
        "}"
        "1 2 3 4 sum4"
    );
    REQUIRE(top == 10);
}

TEST_CASE("Nested functions throws no errors", "[nested functions]") {
    int top = get_top(
        "FUNC {p} {} {"
        "    FUNC {q} {} {"
        "        FUNC {r} {} {"
        "            8"
        "        }"
        "        r"
        "    }"
        "    q"
        "}"
        "p"
    );
    REQUIRE(top == 8);
}

TEST_CASE("Variables can be assigned and used", "[variables]") {
    int top = get_top(
        "10 VAR {x}"
        "1 x +"
    );
    REQUIRE(top == 11);
}

TEST_CASE("Variables can be reassigned", "[variable ressignment]") {
    int top = get_top(
        "10 VAR {x}"
        "x 1 + VAR {x}"
        "x"
    );
    REQUIRE(top == 11);
}

TEST_CASE("Variables can be assigned and used inside functions", "[variables in functions]") {
    int top = get_top(
        "FUNC {seven} {} {7 VAR {x} x} seven"
    );

    REQUIRE(top == 7);
}

TEST_CASE("Incorrect amount of brackets throws error", "[bracket error]") {
    int exit_code = get_exit_code(
        "FUNC {seven} {} {"
        "    7 VAR {my_var}}"
        "    my_var"
        "}"
        "seven"
    );
    REQUIRE(exit_code == 1);
}

TEST_CASE("Incorrect bracket placement throws error", "[bracket error 2]") {
    int exit_code = get_exit_code(
        "FUNC {seven} }{ {"
        "    7 VAR {my_var}"
        "    my_var"
        "}"
        "seven"
    );
    REQUIRE(exit_code == 1);
}

TEST_CASE("Infinite loop can be broken out of", "[infinite loop break]") {
    int top = get_top(
        "0 VAR {x}"
        "LOOP {"
        "    x"
        "    BREAK"
        "}"
    )
    REQURIE(top == 0);
}

