#ifndef TRS_LISP_PRIMITIVES
#define TRS_LISP_PRIMITIVES

#include "trs_lisp.h"

extern long serialctr;

cell *p_ansi_reset(cell *ast, environment *env);
cell *p_ansi(cell *ast, environment *env);
cell *p_exit(cell *ast, environment *env);
cell *p_pass(cell *ast, environment *env);
cell *p_strcmp(cell *ast, environment *env);
cell *p_type(cell *ast, environment *env);

cell *f_add(cell *ast, environment *env);
cell *f_sub(cell *ast, environment *env);
cell *f_mul(cell *ast, environment *env);
cell *f_div(cell *ast, environment *env);
cell *f_equalsign(cell *ast, environment *env);
cell *f_lessthan(cell *ast, environment *env);
cell *f_greaterthan(cell *ast, environment *env);
cell *f_lessthanequal(cell *ast, environment *env);
cell *f_greaterthanequal(cell *ast, environment *env);

cell *f_begin(cell *ast, environment *env);
cell *f_car(cell *ast, environment *env);
cell *f_cdr(cell *ast, environment *env);
cell *f_char_to_integer(cell *ast, environment *env);
cell *f_char_alphabetic(cell *ast, environment *env);
cell *f_char_numeric(cell *ast, environment *env);
cell *f_char_whitespace(cell *ast, environment *env);
cell *f_char_upper_case(cell *ast, environment *env);
cell *f_char_lower_case(cell *ast, environment *env);
cell *f_char_to_integer(cell *ast, environment *env);
cell *f_close_input_port(cell *ast, environment *env);
cell *f_cond(cell *ast, environment *env);
cell *f_cons(cell *ast, environment *env);
cell *f_define(cell *ast, environment *env);
cell *f_define_syntax(cell *ast, environment *env);
cell *f_display(cell *ast, environment *env);
cell *f_do(cell *ast, environment *env);
cell *f_eof_object(cell *ast, environment *env);
cell *f_eqv(cell *ast, environment *env);
cell *f_eq(cell *ast, environment *env);
cell *f_integer_to_char(cell *ast, environment *env);
cell *f_let(cell *ast, environment *env);
cell *f_list(cell *ast, environment *env);
cell *f_load(cell *ast, environment *env);
cell *f_newline(cell *ast, environment *env);
cell *f_number_to_string(cell *ast, environment *env);
cell *f_open_input_file(cell *ast, environment *env);
cell *f_open_output_file(cell *ast, environment *env);
cell *f_quote(cell *ast, environment *env);
cell *f_read_char(cell *ast, environment *env);
cell *f_set(cell *ast, environment *env);
cell *f_string_to_symbol(cell *ast, environment *env);
cell *f_string_eq(cell *ast, environment *env);
cell *f_string_append(cell *ast, environment *env);
cell *f_string(cell *ast, environment *env);
cell *f_string_length(cell *ast, environment *env);
cell *f_substring(cell *ast, environment *env);
cell *f_write(cell *ast, environment *env);
cell *f_write_char(cell *ast, environment *env);

#endif