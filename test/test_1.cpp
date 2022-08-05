#include <catch.hpp>
#include "parser.hpp"

// NOTE: putting each "line" in quotes will not put newlines between the lines. beware of unexpected errors caused by this lack of whitespace.

// helper function so i don't have to type all this out to check top of stack after parsing
Value get_top(std::string raw_src) { 
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
    Value top = get_top(
        "3 5 * 4 + 6 3 / 6 3 / + * 3 7 * -"
    );
    REQUIRE(top == 55);
}

TEST_CASE("Function is delcared and called", "[function]") {
    int exit_code = get_exit_code(
        "func printSeven {"
        "    7 print"
        "}"
        "printSeven"
    );
    REQUIRE(exit_code == 0);
}

TEST_CASE("Function with return value is computed", "[function return]") {
    Value top = get_top(
        "func seven {"
        "    7"
        "}"
        "seven"
    );
    REQUIRE(top == 7);
}

TEST_CASE("Function with return value and arguments is computed", "[function arguments]") {
    Value top = get_top(
        "func sum4 a b c d {"
        "    a b + c + d +"
        "}"
        "1 2 3 4 sum4"
    );
    REQUIRE(top == 10);
}

TEST_CASE("Nested functions throws no errors", "[nested functions]") {
    Value top = get_top(
        "func p {"
        "    func q {"
        "        func r {"
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
    Value top = get_top(
        "10 var x "
        "1 x +"
    );
    REQUIRE(top == 11);
}

TEST_CASE("Variables can be reassigned", "[variable ressignment]") {
    Value top = get_top(
        "10 var x "
        "x 1 + var x "
        "x"
    );
    REQUIRE(top == 11);
}

TEST_CASE("Variables can be assigned and used inside functions", "[variables in functions]") {
    Value top = get_top(
        "func seven {7 var x x} seven"
    );

    REQUIRE(top == 7);
}

TEST_CASE("Incorrect amount of brackets throws error", "[bracket error]") {
    int exit_code = get_exit_code(
        "func seven {"
        "    7 var my_var}"
        "    my_var"
        "}"
        "seven"
    );
    REQUIRE(exit_code == 1);
}

TEST_CASE("Incorrect bracket placement throws error", "[bracket error 2]") {
    int exit_code = get_exit_code(
        "func seven {"
        "    7 var }{my_var}"
        "    my_var"
        "}"
        "seven"
    );
    REQUIRE(exit_code == 1);
}

TEST_CASE("Variables inside functions can be redefined", "[variable redefinition]") {
    Value top = get_top(
        "func seven {7 var x x} seven\n"
        "func eight {8 var x x} eight"
    );

    REQUIRE(top == 8);
}

TEST_CASE("Infinite loop can be broken out of", "[infinite loop break]") {
    Value top = get_top(
        "0 var x "
        "loop {"
        "    x"
        "    break"
        "}"
        "x print"
    );
    REQUIRE(top == 0);
}

TEST_CASE("If statement works with 1 coerced to true", "[if statement]") {
    Value top = get_top(
        "0 "
        "1 if {"
        "    1 1 + 1 -"
        "}"
    );
    REQUIRE(top == 1);
}

TEST_CASE("If statement works with 12394 coerced to true", "[if statement 2]") {
    Value top = get_top(
        "0 "
        "12 1000 * 394 + if { 1 1 * 1 * }"
    );
    REQUIRE(top == 1);
}

TEST_CASE("If statement works with 0 coerced to false", "[if statement 3]") {
    Value top = get_top(
        "0 "
        "0 if {"
        "    1 3 + 3 -"
        "}"
    );
    REQUIRE(top == 0);
}

TEST_CASE("If statement works inside loop", "[if statement in loop]") {
    Value top = get_top(
        "0 5 - var x "
        "loop {"
        "    x"
        "    x if {break}"
        "    x 1 + var x "
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
    Value top = get_top(
        "4 3 %"
    );
    REQUIRE(top == 1);
}

TEST_CASE("Pow is computed", "[pow]") {
    Value top = get_top(
        "2 5 ^"
    );
    REQUIRE(top == 32);
}

TEST_CASE("Addition is computed", "[addition]") {
    Value top = get_top(
        "3 4 +"
    );
    REQUIRE(top == 7);
}

TEST_CASE("Subtraction is computed", "[subtraction]") {
    Value top = get_top(
        "3 4 -"
    );
    REQUIRE(top == -1);
}

TEST_CASE("Multiplication is computed", "[multiplication]") {
    Value top = get_top(
        "7 8 *"
    );
    REQUIRE(top == 56);
}

TEST_CASE("Division is computed", "[division]") {
    Value top = get_top(
        "8 2 /"
    );
    REQUIRE(top == 4);
}

TEST_CASE("less than when less", "[less than]") {
    Value top = get_top(
        "3 4 <"
    );
    REQUIRE(top == 1);
}

TEST_CASE("less than when equal") {
    Value top = get_top(
        "7 7 <"
    );
    REQUIRE(top == 0);
}

TEST_CASE("less than when greater") {
    Value top = get_top(
        "5 4 <"
    );
    REQUIRE(top == 0);
}

TEST_CASE("greater than when greater", "[greater than]") {
    Value top = get_top(
        "8 7 >"
    );
    REQUIRE(top == 1);
}

TEST_CASE("greater than when equal") {
    Value top = get_top(
        "7 7 >"
    );
    REQUIRE(top == 0);
}

TEST_CASE("greater than when less") {
    Value top = get_top(
        "1 2 >"
    );
    REQUIRE(top == 0);
}

TEST_CASE("equal to") {
    Value top = get_top(
        "2 2 ="
    );
    REQUIRE(top == 1);
}

TEST_CASE("equal to fails") {
    Value top = get_top(
        "2 3 ="
    );
    REQUIRE(top == 0);
}

TEST_CASE("not equal to") {
    Value top = get_top(
        "2 3 = !"
    );
    REQUIRE(top == 1);
}

TEST_CASE("not false") {
    Value top = get_top(
        "0 !"
    );
    REQUIRE(top == 1);
}

TEST_CASE("not true") {
    Value top = get_top(
        "1 !"
    );
    REQUIRE(top == 0);
}

TEST_CASE("not equal to fails") {
    Value top = get_top(
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
    Value top = get_top(
        "0 ! !"
    );
    REQUIRE(top == 0);
}

TEST_CASE("not not true") {
    Value top = get_top(
        "1 ! !"
    );
    REQUIRE(top == 1);
}

TEST_CASE("or true") {
    Value top = get_top(
        "1 1 |"
    );
    REQUIRE(top == 1);
}

TEST_CASE("or false") {
    Value top = get_top(
        "0 1 |"
    );
    REQUIRE(top == 1);
}

TEST_CASE("or false 2") {
    Value top = get_top(
        "1 0 |"
    );
    REQUIRE(top == 1);
}

TEST_CASE("or false 3") {
    Value top = get_top(
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
    Value top = get_top(
        "1 1 &"
    );
    REQUIRE(top == 1);
}

TEST_CASE("and false") {
    Value top = get_top(
        "0 1 &"
    );
    REQUIRE(top == 0);
}

TEST_CASE("and false 2") {
    Value top = get_top(
        "1 0 &"
    );
    REQUIRE(top == 0);
}

TEST_CASE("and false 3") {
    Value top = get_top(
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
