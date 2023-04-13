# trs-lisp
Term Rewriting System Lisp  

A dynamically typed lisp-like language based primarily on term rewriting. Functions modify the Abstract Syntax Tree (AST) instead of allocating memory from the heap. A multi-level Environment is provided to enable global and lexically scoped variables.  

## Term Rewriting
This project is intended to implement a usable programming language primarily with Term Rewriting. The S-Expression is chosen for the language's syntax because it's easily parsed into an AST. Macros, defined in the source language, identify an Expression and an Expansion. When a Macro Expression matches an S-Expression in the source code, Variables within the Macro Expression are Unified with elements of the matched S-Expression. Those same Variables in The Macro Expansion are then replaced with the Unified values and the result completely replaces the matched S-Expression in the source code.  

This method is utilized in two ways...

### Functions
Functions are implemented as Macros. The Eval function first looks for matching Functions before proceeding to evaluate Operations. Functions are given priority by Eval, which will otherwise try to work from the leaf nodes back up the AST to the trunk, running Operations as they are encountered so that when any Operation is evaluated, it is guaranteed that all node downstream (toward the leaf nodes) have already been evaluated.

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


## Primitives

### Operations
The following R5RS procedures are implemented as primitive operations:
- \+ (add)
- \- (subtract)
- \* (multiply)
- / (divide)
- car
- cdr
- char-alphabetic?
- char-numeric?
- char-whitespace?
- char-upper-case?
- char-lower-case?
- char->integer
- cons
- defun
- display
- integer->char
- newline
- readchar
- string?
- string-length
- write

### Functions
The following R5RS procedures are implemented as primitive funcations:
- define