#ifndef TRS_LISP_PRIMITIVES
#define TRS_LISP_PRIMITIVES


#include "trs_lisp.h"

extern long serialctr;


cell *f_add(cell *ast, environment *env);
cell *f_sub(cell *ast, environment *env);
cell *f_mul(cell *ast, environment *env);
cell *f_div(cell *ast, environment *env);
cell *f_equalsign(cell *ast, environment *env);
cell *f_lessthan(cell *ast, environment *env);
cell *f_greaterthan(cell *ast, environment *env);
cell *f_lessthanequal(cell *ast, environment *env);
cell *f_greaterthanequal(cell *ast, environment *env);

cell *f_car(cell *ast, environment *env);
cell *f_cdr(cell *ast, environment *env);
cell *f_char_alphabetic(cell *ast, environment *env);
cell *f_char_numeric(cell *ast, environment *env);
cell *f_char_whitespace(cell *ast, environment *env);
cell *f_char_upper_case(cell *ast, environment *env);
cell *f_char_lower_case(cell *ast, environment *env);
cell *f_char_to_integer(cell *ast, environment *env);
cell *f_cond(cell *ast, environment *env);
cell *f_cons(cell *ast, environment *env);
cell *f_define(cell *ast, environment *env);
cell *f_define_syntax(cell *ast, environment *env);
cell *f_display(cell *ast, environment *env);
cell *f_do(cell *ast, environment *env);
cell *f_integer_to_char(cell *ast, environment *env);
cell *f_let(cell *ast, environment *env);
cell *f_list(cell *ast, environment *env);
cell *f_load(cell *ast, environment *env);
cell *f_newline(cell *ast, environment *env);
cell *f_read_char(cell *ast, environment *env);
cell *f_string(cell *ast, environment *env);
cell *f_string_length(cell *ast, environment *env);
cell *f_write(cell *ast, environment *env);
cell *f_write_char(cell *ast, environment *env);

#endif