# trs-lisp
Term Rewriting System Lisp  

A dynamically typed lisp-like language based primarily on term rewriting and self-modifying code. Functions modify the Abstract Syntax Tree (AST) instead of allocating memory from the heap. A multi-level Environment is provided to enable global and lexically scoped variables.  

## Term Rewriting
This project is primarily intended to implement a usable programming language with Term Rewriting. The S-Expression is chosen for the language's syntax because it's easily parsed into an AST. Macros, defined in the source language, identify an Expression and an Expansion. When a Macro Expression matches an S-Expression in the source code, Variables within the Macro Expression are Unified with elements of the matched S-Expression. Those same Variables in The Macro Expansion are then replaced with the Unified values and the result completely replaces the matched S-Expression in the source code.  

This method is utilized in two ways...

### Functions
User Functions are implemented as Macros. These macros modify the AST from the original source code, essentially (perhaps recursively) replacing 'function calls' with primitive operators.

R5RS, this project's source of function and operation names and behaviors (though we will take many liberties), describes one use of **define** as <code>(define ({variable} {formals}) {body})</code>.  

Our **define** has the signature <code>(define ({functionname} {arglist}) {expansion})</code> where expression may contain Variables - included in arglist - to be unified during the resolution process. Variables - for Term Rewritingn Macros only - are identifiers beginning with an Uppercase Letter.

>For example, the follow Function...  
><code>(define (++ A B C) (+ A (+ B C)) )</code>  
>Will transform the following source code...  
><code>(++ 1 2 3)</code>  
>into...  
><code>(+ 1 (+ 2 3))</code>

### Read Macros
The second use of Term Rewriting Macros will be for parsing source code, prior to the generation of an AST by the language's parser.  
R5RS describes <code>(define-syntax {keyword} {transformer})</code>.

## Self Modifying Code 
A second objective of this project is to implement a reliable system of self-modifying code. To this end, instead of allocating memory on the heap or using the stack, this system always works with the AST originally created by parsing the provided source code. Primitive Operations that may be expected to allocate memory dynamically in other languages will modify the AST in trs-lisp, essentially allocating all necessary memory within the existing AST, releasing nodes that are no longer needed, etc.

## Procedures

### Primitives
The following procedures are provided in order to facilitate implentation of other procedures via macros:  
- ansi - accepts one numeric argument X and prints ansi code for color X
- ansi-reset - prints ansi reset code
- exit - exits with no error
- noop - do nothing
- strcmp - returns the result of a C strcmp of two arguments
- type - returns the type of its first argument as a string

The following R5RS procedures are implemented as primitive operators:
- \+ (add)
- \- (subtract)
- \* (multiply)
- / (divide)
- =
- <
- \>
- <=
- \>=
- begin
- car
- cdr
- char-alphabetic?
- char-numeric?
- char-whitespace?
- char-upper-case?
- char-lower-case?
- char->integer
- close-input-port
- close-output-port
- cons
- define
- do
- display
- eof-object?
- eqv?
- eq?
- integer->char
- let
- list 
- load
- newline
- number->string
- open-input-file
- open-output-file
- quote (or just ' )
- readchar
- set!
- string
- string-append
- string-length
- string=?
- substring
- write  

### Macros
The following procedures are implemented in stdlib.trsl:  
- stream?

The following R5RS procedures are implemented as macros in stdlib.trsl:  
- boolean?
- cadr  
- caddr
- equal?
- if
- negative?
- number?
- positive?
- string?
- string<?
- string\>?
- string<=?
- string\>=?
- symbol?
- zero?

## References

Descriptions of R5RS functions can be found at [Scheme Revised(5) Report on the Algorithmic Language Scheme's Variables and Procedures page](https://www-sop.inria.fr/indes/fp/Bigloo/doc/r5rs-12.html)