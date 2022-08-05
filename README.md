# pringlelang

An interpreted, procedural, [stack-based](https://en.wikipedia.org/wiki/Stack-oriented_programming), [reverse Polish notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation) programming language.

**Note that the README is a work in progress. It is not completely accurate.**

## Syntax

A typical pringlelang expression looks like `arg1 arg2 ... argN function`, where the arguments are other expressions; or values, which are integers /\d+/, strings /".*"/, or identifiers (variable or function) /[a-zA-Z]\w*/. For special functions, there may be trailing special statements (e.g. in `2 var x`, x is a special statement, in `loop {...}`, the curly braces with the expressions inside them is a special statement).

// add something about how values are appended to the stack

### Variable declaration and assignment
You must declare and assign a variable at the same time. The result of the expression before the `var` function/keyword is the value of the variable and the special statement after `var` is the variable name (which must be in the form of an identifier). Example: `3 4 + var n` will assign the value `7` to the variable `n`. Note that all variables are in the global scope.

### Function declaration
You declare functions in the form `func arg1 arg2 ... argN {...}`. `func` is the function declaration keyword, and it's followed by space separated args (these args will be replaced with their actual values when the function is called), which is followed by the function body wrapped in curly braces. You can return values just by adding them to the stack.

### Loops
Add an infinite loop by using `loop {...}`. `loop` is the infinite loop function/keyword, and it's followed by the loop body wrapped in curly braces. Use the `break` keyword to break out of the infinite loop.

### If statements
Add an if statement by using `expr if {...}`. expr here would be an expression that would be coerced into a boolean by if. If that boolean is true, the if body wrapped in curly braces will run.

### Example expressions
* `3 5 + print`
* `6 3 / 10 2 / + 3 - print`
* `func square x { x x * }`
