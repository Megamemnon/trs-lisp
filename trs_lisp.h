#ifndef TRS_LISP
#define TRS_LISP

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <gc.h>

// #define DEBUG
// #define DEBUG_VERBOSE

#define MAX_SYMBOL_LENGTH 50
#define INPUT_BUFFER_LENGTH 256

typedef enum TOKENTYPE{
    openparen, closeparen, tsymbol, tnumber, tstring, quote, eof
} tokentype;

typedef struct TOKEN{
    char symbol[MAX_SYMBOL_LENGTH];
    tokentype type;
}token;

typedef enum VALTYPE{number, symbol, string, nil} valtype;

typedef struct CELL{
    long serial;
    char *symbol;
    float number;
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

macro *newMacro(char *name, cell *expression, cell *expansion);
void addMacro(macro *m);
void addFunction(macro *m);

cell *eval(cell *ast, environment *env);

void loadfile(const char *filename, environment *env);

#endif