### Arithmetic Eval Project Report

To parse Polish notation expressions, I split the work into tokenizing, parsing, and evaluating.

Tokenising was very simple as I just had to keep track of whether I am reading a number and turn that into
a number token whenever another kind of token or a space is read.

To parse the tokens, I used the fact that ALL operators are binary and there aren't any edge cases except for integer literals to
create a recursive function that outputs a SyntaxNode object and calls itself to get the object's children.
To be able to return the value, I heap-allocated SyntaxNodes using std::unique_ptr and move semantics to transfer ownership.
Since I was already using heap-allocations, I used a dynamic dispatch interface that both LiteralNode and BinaryOperatorNode implement.

Evaluating the expression was very simple as I just had to call the root node's evaluate method.

A limitation of the program is that negative numbers are not supported. This is because minus is always a binary operator,
so they have to be written as (- 0 X) instead of -X.

### Running the program

```
Enter an expression: (*5(*4(*3(*2 1))))
Result: 120
Enter an expression: (- 0 (*5(*4(*3(*2 1)))))
Result: -120
Enter an expression: (      +        2        (/2 2)) 
Result: 3
Enter an expression: (/ 9999 1000)
Result: 9
Enter an expression: (% 9999 1000)
Result: 999
Enter an expression: exit

Process finished with exit code 0

```