#include <catch.hpp>
#include "parser.hpp"

// NOTE: putting each "line" in quotes will not put newlines between the lines. beware of unexpected errors caused by this lack of whitespace.

// helper function so i don't have to type all this out to check top of stack after parsing
int get_top(std::string raw_src) { 
    Parser parser;
    SourceCode src = SourceCode(raw_src);
    parser.parse(src);

    return parser.try_peek();
}

int get_exit_code(std::string raw_src) {
    Parser parser;
    SourceCode src = SourceCode(raw_src);
    
    return parser.parse(src);
}

//the second parameter here is the label for the category of test.
// TODO: organize these better and label by category/scope

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

TEST_CASE("Variables inside functions can be redefined", "[variable redefinition]") {
    int top = get_top(
        "FUNC {seven} {} {7 VAR {x} x} seven\n"
        "FUNC {eight} {} {8 VAR {x} x} eight"
    );

    REQUIRE(top == 8);
}

TEST_CASE("Infinite loop can be broken out of", "[infinite loop break]") {
    int top = get_top(
        "0 VAR {x}"
        "LOOP {"
        "    x"
        "    BREAK"
        "}"
    );
    REQUIRE(top == 0);
}

TEST_CASE("If statement works with 1 coerced to true", "[if statement]") {
    int top = get_top(
        "0 "
        "1 IF {"
        "    1 1 + 1 -"
        "}"
    );
    REQUIRE(top == 1);
}

TEST_CASE("If statement works with 12394 coerced to true", "[if statement 2]") {
    int top = get_top(
        "0 "
        "12 1000 * 394 + IF { 1 1 * 1 * }"
    );
    REQUIRE(top == 1);
}

TEST_CASE("If statement works with 0 coerced to false", "[if statement 3]") {
    int top = get_top(
        "0 "
        "0 IF {"
        "    1 3 + 3 -"
        "}"
    );
    REQUIRE(top == 0);
}

TEST_CASE("If statement works inside loop", "[if statement in loop]") {
    int top = get_top(
        "0 5 - VAR {x}"
        "LOOP {"
        "    x"
        "    x IF {BREAK}"
        "    x 1 + VAR {x}"
        "    x print"
        "}"
    );
    REQUIRE(top == 1);
}

TEST_CASE("Invalid amount of operands (0) throws error", "[operand error0]") {
    int exit_code = get_exit_code(
        "+"
    );
    REQUIRE(exit_code == 1);
}

TEST_CASE("Invalid amount of operands (1) throws error", "[operand error1]") {
    int exit_code = get_exit_code(
        "5 +"
    );
    REQUIRE(exit_code == 1);
}

TEST_CASE("Modulo is computed", "[modulo]") {
    int top = get_top(
        "4 3 %"
    );
    REQUIRE(top == 1);
}

TEST_CASE("Pow is computed", "[pow]") {
    int top = get_top(
        "2 5 ^"
    );
    REQUIRE(top == 32);
}

TEST_CASE("Addition is computed", "[addition]") {
    int top = get_top(
        "3 4 +"
    );
    REQUIRE(top == 7);
}

TEST_CASE("Subtraction is computed", "[subtraction]") {
    int top = get_top(
        "3 4 -"
    );
    REQUIRE(top == -1);
}

TEST_CASE("Multiplication is computed", "[multiplication]") {
    int top = get_top(
        "7 8 *"
    );
    REQUIRE(top == 56);
}

TEST_CASE("Division is computed", "[division]") {
    int top = get_top(
        "8 2 /"
    );
    REQUIRE(top == 4);
}

TEST_CASE("less than when less", "[less than]") {
    int top = get_top(
        "3 4 <"
    );
    REQUIRE(top == 1);
}

TEST_CASE("less than when equal") {
    int top = get_top(
        "7 7 <"
    );
    REQUIRE(top == 0);
}

TEST_CASE("less than when greater") {
    int top = get_top(
        "5 4 <"
    );
    REQUIRE(top == 0);
}

TEST_CASE("greater than when greater", "[greater than]") {
    int top = get_top(
        "8 7 >"
    );
    REQUIRE(top == 1);
}

TEST_CASE("greater than when equal") {
    int top = get_top(
        "7 7 >"
    );
    REQUIRE(top == 0);
}

TEST_CASE("greater than when less") {
    int top = get_top(
        "1 2 >"
    );
    REQUIRE(top == 0);
}

TEST_CASE("equal to") {
    int top = get_top(
        "2 2 ="
    );
    REQUIRE(top == 1);
}

TEST_CASE("equal to fails") {
    int top = get_top(
        "2 3 ="
    );
    REQUIRE(top == 0);
}

TEST_CASE("not equal to") {
    int top = get_top(
        "2 3 = !"
    );
    REQUIRE(top == 1);
}

TEST_CASE("not false") {
    int top = get_top(
        "0 !"
    );
    REQUIRE(top == 1);
}

TEST_CASE("not true") {
    int top = get_top(
        "1 !"
    );
    REQUIRE(top == 0);
}

TEST_CASE("not equal to fails") {
    int top = get_top(
        "2 2 = !"
    );
    REQUIRE(top == 0);
}

TEST_CASE("not with no operand") {
    int exit_code = get_exit_code(
        "!"
    );
    REQUIRE(exit_code == 1);
}

TEST_CASE("not not false") {
    int top = get_top(
        "0 ! !"
    );
    REQUIRE(top == 0);
}

TEST_CASE("not not true") {
    int top = get_top(
        "1 ! !"
    );
    REQUIRE(top == 1);
}

TEST_CASE("or true") {
    int top = get_top(
        "1 1 |"
    );
    REQUIRE(top == 1);
}

TEST_CASE("or false") {
    int top = get_top(
        "0 1 |"
    );
    REQUIRE(top == 1);
}

TEST_CASE("or false 2") {
    int top = get_top(
        "1 0 |"
    );
    REQUIRE(top == 1);
}

TEST_CASE("or false 3") {
    int top = get_top(
        "0 0 |"
    );
    REQUIRE(top == 0);
}

TEST_CASE("or with no operand") {
    int exit_code = get_exit_code(
        "|"
    );
    REQUIRE(exit_code == 1);
}

TEST_CASE("and true") {
    int top = get_top(
        "1 1 &"
    );
    REQUIRE(top == 1);
}

TEST_CASE("and false") {
    int top = get_top(
        "0 1 &"
    );
    REQUIRE(top == 0);
}

TEST_CASE("and false 2") {
    int top = get_top(
        "1 0 &"
    );
    REQUIRE(top == 0);
}

TEST_CASE("and false 3") {
    int top = get_top(
        "0 0 &"
    );
    REQUIRE(top == 0);
}

TEST_CASE("and with no operand") {
    int exit_code = get_exit_code(
        "&"
    );
    REQUIRE(exit_code == 1);
}
