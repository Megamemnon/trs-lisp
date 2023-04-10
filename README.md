# trs-lisp
Term Rewriting System Lisp  

A dynamically typed lisp-like language based primarily on term rewriting. Functions modify the Abstract Syntax Tree (AST) instead of allocating memory from the heap. A multi-level Environment is provided to enable global and lexically scoped variables.  

## Primitives
The following R5RS procedures are implemented as primitives:
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

