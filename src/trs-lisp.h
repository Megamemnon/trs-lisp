/*************************************************************************
  trs-lisp
  Copyright (c) 2023 Brian O'Dell
  
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  This Source Code Form is "Incompatible With Secondary Licenses", as
  defined by the Mozilla Public License, v. 2.0.

*************************************************************************/

#ifndef TRS_LISP
#define TRS_LISP

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <gc.h>

#define TEXTBOX2

#ifdef TEXTBOX2
#include "termbox2.h"
#endif

#define R5RS_PRIMITIVES

#define DEBUG
#define DEBUG_FILE_1 "/home/brian/git/trs-lisp/lib/stdlib.trsl"
#define DEBUG_FILE_2 "/home/brian/git/trs-lisp/test/unittests.trsl"
// #define DEBUG_FILE_1 "/home/brian/git/trs-lisp/test/stdlib0.trsl"
// #define DEBUG_FILE_2 "/home/brian/git/trs-lisp/edit.trsl"
// #define DEBUG_VERBOSE


#define MAX_SYMBOL_LENGTH 50
#define INPUT_BUFFER_LENGTH 256

typedef enum TOKENTYPE{
    openparen, closeparen, tsymbol, tnumber, tstring, tboolean, quote, eof
} tokentype;

typedef struct TOKEN{
    char symbol[MAX_SYMBOL_LENGTH];
    tokentype type;
}token;

typedef enum VALTYPE{number, symbol, string, boolean, stream, nil} valtype;

typedef struct CELL{
    long serial;
    char *symbol;
    float number;
    FILE *stream;
    valtype type;
    struct CELL *contents;
    struct CELL *next;
} cell;

typedef struct UNIFIER
{
    char *variable;
    cell *boundterm;
    struct UNIFIER *next;
}unifier;

typedef struct RESOLUTION{
    unifier *unifier;
    cell *matchedcell;
    struct RESOLUTION *next;
}resolution;

typedef struct ENVIRONMENT{
    cell *bindings;
    cell *lastbinding;
    struct ENVIRONMENT *parent;
}environment;

typedef struct PRIMITIVE
{
    char *name;
    cell * (*f)(cell *, environment *);
    struct PRIMITIVE *next;
} primitive;

typedef struct MACRO{
    char *name;
    cell *expression;
    cell *expansion;
    struct MACRO *next;
} macro;

cell *newcell(long serial, char *symbol, float number, valtype type);
cell *copyCellDeep(cell *cl);
char *getStringfromAST(cell *ast);

void bindVar(char *var, cell *bound, environment *env);
void setVar(char *var, cell *bound, environment *env);
cell *getVarBinding(char *var, environment *env);

primitive *getPrimitive(char *name);

macro *newMacro(char *name, cell *expression, cell *expansion);
void addMacro(macro *m);
void addFunction(macro *m);

environment *newenvironment(environment *parent);

cell *eval(cell *ast, environment *env);

void loadfile(const char *filename, environment *env);

#endif