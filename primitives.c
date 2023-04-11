
#include "primitives.h"

/* Functions */

cell *f_cond(cell *ast, environment *env){
    return NULL;
}

cell *f_do(cell *ast, environment *env){
    return NULL;
}


cell *f_let(cell *ast, environment *env){
    return NULL;
}


/* Operators */

cell *f_car(cell *ast, environment *env){
    if(ast->next){
        cell *newast=copyCellDeep(ast->next);
        newast->next=NULL;
        return newast;
    }
    return NULL;
}

cell *f_cdr(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->next) {
            cell *newast=copyCellDeep(ast->next->next);
            return newast;
        }
    }
    return NULL;
}

cell *f_char_alphabetic(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(isalpha(ast->next->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_numeric(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(isdigit(ast->next->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_whitespace(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(isspace(ast->next->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_upper_case(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(isupper(ast->next->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_lower_case(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(islower(ast->next->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_to_integer(cell *ast, environment *env){
    if(ast->next){
        char c=ast->next->symbol[0];
        char *buff=(char *)GC_malloc( sizeof(char)*4+1);
        int n=sprintf(buff, "%d", c);
        buff[n+1]=0;
        cell *cl=newcell(serialctr++, buff, c, number);
        return cl;
    }
    return NULL;
}

cell *f_cons(cell *ast, environment *env){
    if(ast->next){
        cell *newast=copyCellDeep(ast->next);
        return newast;
    }
    return NULL;
}

cell *f_defun(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->next) {
            cell *expression=copyCellDeep(ast->next);
            cell *expansion=copyCellDeep(ast->next->next);
            macro *m=newMacro(ast->symbol, expression, expansion);
            addFunction(m);
        }
    }
    return NULL;
}

cell *f_display(cell *ast, environment *env){
    if(ast->next){
        printf("%s", ast->next->symbol);
    }
    return NULL;
}

cell *f_integer_to_char(cell *ast, environment *env){
    if(ast->next){
        char *c=(char *)GC_malloc(2);
        c[0]=ast->next->number;
        cell *cl=newcell(serialctr++, c, 0, string);
        return cl;
    }
    return NULL;
}

cell *f_newline(cell *ast, environment *env){
    printf("\n");
    return NULL;
}

cell *f_readchar(cell *ast, environment *env){
    char *c=(char *)GC_malloc(2);
    c[0]=getchar();
    cell *cl=newcell(serialctr++, c, 0, string);
    return cl;
}

cell *f_string(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(ast->next->type==string){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_string_length(cell *ast, environment *env){
    if(ast->next){
        int l=strlen(ast->next->symbol);
        char *buff=(char *)GC_malloc(sizeof(char)*10+1);
        int n=sprintf(buff, "%d", l);
        buff[n+1]=0;
        cell *cl=newcell(serialctr++, buff, l, number);
        return cl;
    }
    return NULL;
}

cell *f_write(cell *ast, environment *env){
    if(ast->next){
        char *f=getStringfromAST(ast->next);
        printf("%s", f);
    }
    return NULL;
}
