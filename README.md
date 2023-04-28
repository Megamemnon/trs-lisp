# trs-lisp
Term Rewriting System Lisp  

A dynamically typed lisp-like language based primarily on term rewriting and self-modifying code. Functions modify the Abstract Syntax Tree (AST) instead of allocating memory from the heap. A multi-level Environment is provided to enable global and lexically scoped variables.  

## Term Rewriting
This project was primarily intended to implement a usable programming language with Term Rewriting. The S-Expression was chosen for the language's syntax because it's easily parsed into an AST. Macros, defined in the source language, identify an Expression and an Expansion. When a Macro Expression matches an S-Expression in the source code, Meta Variables (identifed by uppercase first letters) within the Macro Expression are Unified with elements of the matched S-Expression. Those same Variables in The Macro Expansion are then replaced with the Unified values and the result completely replaces the matched S-Expression's AST structure.  

### Macros
User Functions can be implemented as Macros. These macros modify the AST from the original source code, essentially (perhaps recursively) replacing 'function calls' with primitive operators.

R5RS, this project's source of function and operation names and behaviors (though we will take many liberties), describes one use of **define-syntax** as <code>(define {keyword} {transformer})</code>.  

Our **define-syntax** has the signature <code>(define {functionname} ({functionname} {arglist}) {expansion})</code> where expression may contain Variables - included in arglist - to be unified during the resolution process. Variables - for Term Rewritingn Macros only - are identifiers beginning with an Uppercase Letter.

>For example, the follow Macro...  
><code>(define-syntax ++ (++ A B C) (+ A (+ B C)) )</code>  
>Will transform the following source code...  
><code>(++ 1 2 3)</code>  
>into...  
><code>(+ 1 (+ 2 3))</code>

### Self Modifying Code 
Instead of allocating memory on the heap or using the stack, this system always works with the AST originally created by parsing the provided source code (and applying macros). Primitive Operations that may be expected to allocate memory dynamically in other languages will modify the AST in trs-lisp, essentially allocating all necessary memory within the existing AST, releasing nodes that are no longer needed, etc.  

A second type of user defined function (**define-function**) works the same way, binding the function's Meta Variables to parameters provided by the invoking code, and then evaluating the modified function.  


## Evaluation Strategy

-  Source code is tokenized
-  A single s-expression's Tokens are parsed into an AST
-  Macros (defined by define-syntax) are applied to the AST
-  AST is Evaluated  
- - Strings, Numbers, and Booleans evaluate to themselves 
- - Symbols are looked up in the environment (variables)  
- - - Bound symbols are replaced with the AST to which they were bound  
- - Symbols are looked up in primitive procedures  
- - - primitive is invoked and will evaluate AST elements as needed  
- - Symbols are looked up in user defined functions  
- - - Meta variables in the function's Expansion are replaced by the provided parameters  
- - - The modified function Expansion is, itself, evaluated  
- - Unmatched Symbols evaluate to themselves  

### Primitives
The following procedures are implemented as primitives in order to facilitate implentation of user functions and macros:  

(Upper Case letters represent parameters.)  

| Procedure | Return Type | Description |
| --- | --- | --- | 
| **ansi-code** | nil | print ansi escape sequence "\033[" and A |
| **ansi-reset** | nil | prints ansi escape sequence for reset |
| **define-function** | nil | separated from R5RS's define can be used to define both Variables and Functions |
| **exit** | nil | exits with no error |
| **noop** | nil | do nothing |
| **strcmp** | number | returns the result of a C strcmp of A and B |
| **Type** |string | returns the type of its first argument as a string |

The following R5RS procedures are implemented as primitive operators:  

| Procedure | Return Type | Description |
| --- | --- | --- | 
| **\+** | number | A + B |
| **\-** | number | A \- B |
| **\*** | number | A * B | 
| **/** | number | A / B| 
| **=** | boolean |  #t if A = B | 
| **<** | boolean | A < B |
| **\>** | boolean | A \> B | 
| **<=** | boolean | A <= B |
| **\>=** | boolean | A \>= B |
| **begin** | ast | eval args sequentially |
| **car** | ast | returns first item in list |
| **cdr** | ast | returns everything after first item in list |
| **char-alphabetic?** | boolean | #t if first char is alpha | 
| **char-numeric?** | boolean | #t is first char is a digit |
| **char-whitespace?** | boolean | #t if first char is whitespace |
| **char-upper-case?** | boolean | #t if first char is upper case letter |
| **char-lower-case?** | boolean | #t if first char is lower case letter |
| **char->integer** | number | returns ascii value of first char |
| **close-input-port** | nil | closes a stream |
| **close-output-port** | nil | closes a stream |
| **cons** | ast | (cons A B) -> (A B) |
| **define** | nil | A (variable) is bound to B in current environment | 
| **define-syntax** | nil | A (macro name) B (expression with meta vars) C (expansion with Meta vars)
| **do** | ast | iteration routine |
| **display** | nil | prints evaluated A to screen |
| **eof-object?** | boolean | #t if A is EOF |
| **eqv?** | boolean | #t if A = B |
| **eq?** | boolean | #t if A = B |
| **integer->char** | string | converts integer of A to an ascii char |
| **let** | ast | creates new env, defines vars and evals expressions |
| **list** | ast | returns arguments as a list |
| **load** | ast | loads and evaluates file path name A in current environment |
| **newline** | nil | prints new line to screen |
| **number->string** | string | converts A to a string (base 10) |
| **open-input-file** | stream | opens file path name A for input |
| **open-output-file** | stream | opens file path name A for output |
| **quote** (or just **'** ) | ast | arguments will not be evaluated |
| **read-char** | string | reads char from stdin unless a stream B is provided |
| **set!** | nil | binds variable A to B; A must already be defined |
| **string** | string | converts A to string |
| **string-append** | string | concatenates all arguments as strings |
| **string-length** | number | length of string A |
| **string=?** | boolean | #t if A = B |
| **substring** | string | returns portion of A string starting at B (inclusive) until C (exclusive) |
| **write** | nil | prints s-expression of A to screen |
| **write-char** | nil | prints first char in A to screen or stream if B is provided |

### Functions
The following procedures are implemented in stdlib.trsl:

| Procedure | Return Type | Description |
| --- | --- | --- | 
| **stream?** | boolean | #t if A is a stream |

The following R5RS procedures are implemented as functions in stdlib.trsl:

| Procedure | Return Type | Description |
| --- | --- | --- |
| **boolean?** | boolean | #t if A is boolean |
| **cadr** | ast | car(cdr (A B C ...)) | 
| **caddr** | ast | car(cdr (cdr (A B C ...))) |
| **equal?** | boolean | #t if A = B |
| **if** | ast | (if A B C) where A is boolean, eval B if #t or C if #f |
| **negative?** |  boolean | #t if A < 0 | 
| **number?** |  boolean | #t if A is number |
| **positive?** |  boolean | #t if A \> 0 |
| **string?** |  boolean | #t if A is string |
| **string<?** |  boolean | #t if A is alphabetically before B |
| **string\>?** |  boolean | #t if A is alphabetically after B |
| **string<=?** |  boolean | #t if A is alphabetically before or equal to B |
| **string\>=?** |  boolean | #t if A is alphabetically after or equal to B |
| **symbol?** |  boolean | #t if A is symbol |
| **zero?** | boolean | #t if A is 0 |

## References

Descriptions of R5RS functions can be found at [Scheme Revised(5) Report on the Algorithmic Language Scheme's Variables and Procedures page](https://www-sop.inria.fr/indes/fp/Bigloo/doc/r5rs-12.html)