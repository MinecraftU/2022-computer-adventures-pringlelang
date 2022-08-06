# pringlelang

An interpreted, procedural, [stack-based](https://en.wikipedia.org/wiki/Stack-oriented_programming), [reverse Polish notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation) programming language.

#### :memo: **note:** this README is a work in progress. Some features detailed here may still be in development



## How to build and run pringle
1.
2.
3.



## Syntax
### Example expressions 
* `3 5 + print`
* `6 3 / 10 2 / + 3 - print`
* `func square x { x x * }`

A typical pringlelang expression looks like `arg1 arg2 ... argN function`, where the arguments are other expressions; or values, which are integers ``/\d+/``, strings ``/".*"/``, or identifiers (variable or function) ``/[a-zA-Z]\w*/``. For special functions, there may be trailing special statements (e.g. in `2 var x`, x is a special statement, in `loop {...}`, the curly braces with the expressions inside them is a special statement).
  
  
### Operators
#### Math
- `+` addition
- `-` subtraction
- `*` multiplicated
- `/` division
- `%` modulo

#### Boolean logic
- `&` and
- `|` or
- `!` not
- `=` equals (comparison)

#### Stack manipulations (WIP)
Similar to [Forth](https://www.forth.com/starting-forth/2-stack-manipulation-operators-arithmetic/), pringle includes operators that help the programmer rearrange values on the stack
- ``dup`` duplicate the top element
- ``twodup`` duplicate the top two elements on the stack
- ``swap`` swap the positions of the top two elements on the stack
- ``over`` move the third element to the top of the stack


### Types
Currently there are three types of values that can be represented in pringle:
- strings
- integers (signed 32 bit)
  - **note:** positive values are truthy while 0 and negative values are falsy  
  - a negative integer ``-x`` must be written as ``0 x -`` due to the absence of a unary minus operator in pringle
  - for example, the number -5 would be expressed in pringle as ``0 5 -``
- booleans
  - see integers

### Variables
You must declare and assign a variable at the same time. The expression before the `var` function/keyword is the value of the variable and the identifier after `var` is the variable's name. Note that all variables are in the global scope.
* variable declaration and use example: 
```
3 4 + var n 
```
*assigns the value 7 to the variable n*

```
n print
```
*outputs 7*


### Functions
You declare functions in the form `func arg1 arg2 ... argN {...}`. `func` is the function declaration keyword, and it's followed by space separated args (these args will be replaced with their actual values when the function is called), which is followed by the function body wrapped in curly braces. You can return values just by adding them to the stack.


### If statements
Add an if statement by using `expr if {...}`. expr here would be an expression that would be coerced into a boolean by if. If that boolean is true, the if body wrapped in curly braces will run.


### Loops
Add an infinite loop by using `loop {...}`. `loop` is the infinite loop function/keyword, and it's followed by the loop body wrapped in curly braces. Use the `break` keyword to break out of the infinite loop.
* for loop example
```
1 var flag
    loop {
      flag print
      flag 1 + var flag
      flag 10 = if {break}
    }
 ```
*outputs 123456789*
* same example using a stack-oriented programming approach
```
1
loop {
    dup print
    1 +
    dup 10 = if {break}
}
```
*outputs 123456789*

### Comments
Comments in pringle work the same way as python single line comments
```# this is a comment and everything written after the "#" on this line will be ignored```
