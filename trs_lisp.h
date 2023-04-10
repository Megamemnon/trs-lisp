#ifndef TRS_LISP
#define TRS_LISP

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <gc.h>


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

typedef struct MACRO{
    char *name;
    cell *expression;
    cell *expansion;
    struct MACRO *next;
} macro;

cell *newcell(long serial, char *symbol, float number, valtype type);
cell *copyCellDeep(cell *cl);
char *getStringfromAST(cell *ast);
void addMacro(char *name, cell *expression, cell *expansion);

#endif