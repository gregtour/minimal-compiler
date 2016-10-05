## Minimal Compiler

The minimal compiler project is an attempt to make one of the simplest, 
minimalistic languages that compiles to x86-64 assembly and machine code. Currently, the syntax is inspired by C-code with a little bit of influence from the Duck programming language. It is also modeled after Go and Objective-C to some extent. 

The features of the language are, at a top level, based around variable 
declarations and function declarations.

Types are provided after names, with a trailing colon and the type name, 
followed by a semicolon to delimit statements.

####For example:

```
index : int;
string_Var : string;
count_a, count_b, count_c : int;
```

Functions are defined as they are declared. Multiple source files can be included in a project by using the `include` keyword and including the source filename. These source files are then treated as if they are all one piece of source code.

####Example:

```
include "source2.code";
```

Functions are composed of either a single statement or a block of statements. Each statement is made up of individual expressions, assignments, conditionals, or loops. If/else statements are supported. In the case of ambiguous else branches in the case of several in-line if statements, each else statement is paired with the outermost if or conditional possible. This is the opposite behavior of most other programming languages. While-loops are currently the only loop supported, out of simplicity.

Local variable declarations are allowed. Functions may also return a single variable and may have any number of typed parameters. Recursion is allowed. The program's entry point is the `main` function.

Here is an example of what a function might look like in this language.

####Example:
```
main ( /* no params */ ) : int
{
    count_a, count_b : int;
    count_b = 1;

    prints("Print five stars:\n");
    while (count_b <= 5) {
        count_a = 1;
        while (count_a <= count_b)
        {
            prints("*");
        }
        prints("\n");
        count_b++;
    }

    return 0;
}
```

Initially, the only built in functions are intended to be `prints` and `printi`. Here are the function prototypes for each.

####Source:

```
/* displays a string message to stdout */
prints (message : string) : void 0;
```

```
/* prints an integer to stdout */
printi (number : int) : void 0;
```

That is the current goal. The intended platforms are both 64-bit versions of Windows and Linux, with potential support for 32-bit environments as well.

####Goals

Performance.

Support strongly typed, static first class functions with up and downward Funargs.

Maintain a comprehensible stack and memory model.

Be Turing complete.

Exist on a level parallel with C.

Prevent exceptional runtime behavior.

####Links

http://www.animats.com/papers/languages/safearraysforc43.pdf
