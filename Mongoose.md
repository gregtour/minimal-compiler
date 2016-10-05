```
Mongoose Language
-=-=-=-=-=-=-=-=-

Snippet:

  (int)int addr(int i) {
   return (int j)int {
     return (i+j);
   };
  }

  int main() {
    return addr(-2)(1+1);
  }

Mongoose Grammar:

  <0> -> <S>*
  <S> -> include
  <S> -> linker
  <S> -> macro
  <S> -> type definition
  <S> -> static_variable -> <type> <name> \; | <type> <name> = <exp> \;
  <S> -> func_decl -> <type> <name> ( <params> ) \;
  <S> -> func_definition -> <type> <name> ( <params> ) { <St>* }
      <params> -> <param> <param+> | e
      <param+> -> <param> | e
      <param> -> <type> <name>
  <St> -> if (<expr>) { <St>* }
  <St> -> if / else / else if
  <St> -> while ( <expr> ) { <St>* }
  <St> -> do { <St>* } while ( <expr> ) \;
  <St> -> for ( <ForSt> \; <expr> \; <ForSt> ) { <St>* }
  <St> -> switch ( <expr> ) { <switch_label>* }
      <switch_label> -> case <expr> : <St>* | default:
  <St> -> break \;
  <St> -> continue \;
  <St> -> return \; | return <expr> \;
  <St> -> goto <label> \;
  <St> -> <expr> \;

  <expr> -> call\cc <expr>
  <expr> -> <name>
  <expr> -> <value>
  <expr> -> add, subtract, mult, divide, and, or, comparison of <expr>

Differences from ANSI C:

    -auto, char, const, double, enum, extern, float,
    -int, long, register, short, signed, sizeof, static,
    -struct, typedef, union, unsigned, void, volative
    -typecasts
    +mandatory { curly braces }
    +some of the above keywords with different uses
    +call\cc expression

Mongoose Primitive Types:

  integers, doubles, and strings
  void (not used for declared variables)
  auto (lazy compiler trick)

Mongoose Data Types:

  fixed-size arrays
  dynamically sized arrays
  
Mongoose Function Types:

  (parameter types) return type

Mongoose User Types:

    struct <name>
    {
      <type>: <name> \;  **
    }\;

Mongoose Pre-processor:

  The Mongoose preprocessor executes an inline Lisp program to transform the source code.

Mongoose Compiler/Linker:

  include source code
  include header (declarations)
  link object: uses binary header

Mongoose with C:
C with Mongoose:

  Library, macros, and techniques to make extensions to Mongoose in C.
  Useful for creating new abstract data types.
  Mongoose code can be called from other languages when it is built for a C ABI, with special constraints.

Mongoose Static Library:

  Regular expressions.
  Math. Random numbers.
  Other useful libraries.
  Trees, hash tables, heaps.
  Sorting functions.
  File input and output.
  C standard library type features.

Mongoose Runtime Environment:

  Reference counting overhead.
  Dynamic array overhead.
  Funarg object overhead.
  Heap lifting and dropping stack mechanics.
  Call\cc constraints.

Mongoose Code Execution:

  ?

```
