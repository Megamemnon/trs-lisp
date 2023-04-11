#ifndef TRS_LISP_PRIMITIVES
#define TRS_LISP_PRIMITIVES


#include "trs_lisp.h"

extern long serialctr;

cell *f_cond(cell *ast, environment *env);
cell *f_do(cell *ast, environment *env);
cell *f_let(cell *ast, environment *env);

cell *f_car(cell *ast, environment *env);
cell *f_cdr(cell *ast, environment *env);
cell *f_char_alphabetic(cell *ast, environment *env);
cell *f_char_numeric(cell *ast, environment *env);
cell *f_char_whitespace(cell *ast, environment *env);
cell *f_char_upper_case(cell *ast, environment *env);
cell *f_char_lower_case(cell *ast, environment *env);
cell *f_char_to_integer(cell *ast, environment *env);
cell *f_defun(cell *ast, environment *env);
cell *f_cons(cell *ast, environment *env);
cell *f_display(cell *ast, environment *env);
cell *f_integer_to_char(cell *ast, environment *env);
cell *f_newline(cell *ast, environment *env);
cell *f_readchar(cell *ast, environment *env);
cell *f_string(cell *ast, environment *env);
cell *f_string_length(cell *ast, environment *env);
cell *f_write(cell *ast, environment *env);


#endif