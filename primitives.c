
#include "primitives.h"

/* Functions */


cell *f_define(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->contents){
            // define a function macro
            if(ast->next->next) {
                cell *expression=copyCellDeep(ast->next);
                expression->next=NULL;
                cell *expansion=copyCellDeep(ast->next->next);
                if(expression->contents && expansion->contents 
                    && !expression->next && !expansion->next){
                        expression=expression->contents;
                        expansion=expansion->contents;
                    }
                macro *m=newMacro(ast->next->contents->symbol, expression, expansion);
                addFunction(m);
                return NULL;
            }
        } else {
            // define a variable
            if(ast->next->type==symbol){
                if(ast->next->next){
                    bindVar(ast->next->symbol, ast->next->next, env);
                }
            }
        }
    }
    return NULL;
}

cell *f_define_syntax(cell *ast, environment *env){
    return NULL;
}

cell *f_do(cell *ast, environment *env){
    cell *vars=NULL;
    cell *test=NULL;
    cell *command=NULL;
    if(ast->next){
        if(ast->next->contents){
            if(ast->next->contents->contents){
                vars=ast->next->contents->contents;
            }
            if(ast->next->contents->next){
                if(ast->next->contents->next->contents){
                    test=ast->contents->next->contents;     
                }
            }
            if(ast->next->contents->next->next){
                command=ast->next->contents->next->next;
            }
        }
    }
    if(vars && test && command){
        // setup variables
        // loop
        // test
        // step
    }
}


cell *f_let(cell *ast, environment *env){
    return NULL;
}

cell *f_load(cell *ast, environment *env){
    if(ast->next){
        if(ast->type==string || ast->type==symbol){
            loadfile(ast->symbol, env);
        }
    }
    return NULL;
}


/* Operators */

cell *f_add(cell *ast, environment *env){
    float a, b;
    if(ast->next){
        if(ast->next->next){
            b=eval(ast->next->next, env)->number;
        }
        a=eval(ast->next, env)->number;
    }
    char buffer[20];
    int n=sprintf(buffer, "%f", a+b);
    return newcell(serialctr++, buffer, a+b, number);
}

cell *f_sub(cell *ast, environment *env){
    float a, b;
    if(ast->next){
        a=ast->next->number;
        if(ast->next->next){
            b=ast->next->next->number;
        }
    }
    char buffer[20];
    int n=sprintf(buffer, "%f", a-b);
    return newcell(serialctr++, buffer, a-b, number);
}

cell *f_mul(cell *ast, environment *env){
    float a, b;
    a=1;b=1;
    if(ast->next){
        a=ast->next->number;
        if(ast->next->next){
            b=ast->next->next->number;
        }
    }
    char buffer[20];
    int n=sprintf(buffer, "%f", a*b);
    return newcell(serialctr++, buffer, a*b, number);
}

cell *f_div(cell *ast, environment *env){
    float a, b;
    a=1;b=1;
    if(ast->next){
        a=ast->next->number;
        if(ast->next->next){
            b=ast->next->next->number;
        }
    }
    char buffer[20];
    int n=sprintf(buffer, "%f", a/b);
    return newcell(serialctr++, buffer, a/b, number);
}

cell *f_equalsign(cell *ast, environment *env){
    float a, b;
    a=0;b=0;
    if(ast->next){
        a=ast->next->number;
        if(ast->next->next){
            b=ast->next->next->number;
        }
    }
    if(a==b){
        return newcell(serialctr++, "#t", 0, symbol);
    } else {
        return newcell(serialctr++, "#f", 0, symbol);
    }
}

cell *f_lessthan(cell *ast, environment *env){
    float a, b;
    a=0;b=0;
    if(ast->next){
        a=ast->next->number;
        if(ast->next->next){
            b=ast->next->next->number;
        }
    }
    if(a<b){
        return newcell(serialctr++, "#t", 0, symbol);
    } else {
        return newcell(serialctr++, "#f", 0, symbol);
    }
}

cell *f_greaterthan(cell *ast, environment *env){
    float a, b;
    a=0;b=0;
    if(ast->next){
        a=ast->next->number;
        if(ast->next->next){
            b=ast->next->next->number;
        }
    }
    if(a>b){
        return newcell(serialctr++, "#t", 0, symbol);
    } else {
        return newcell(serialctr++, "#f", 0, symbol);
    }
}

cell *f_lessthanequal(cell *ast, environment *env){
    float a, b;
    a=0;b=0;
    if(ast->next){
        a=ast->next->number;
        if(ast->next->next){
            b=ast->next->next->number;
        }
    }
    if(a<=b){
        return newcell(serialctr++, "#t", 0, symbol);
    } else {
        return newcell(serialctr++, "#f", 0, symbol);
    }
}

cell *f_greaterthanequal(cell *ast, environment *env){
    float a, b;
    a=0;b=0;
    if(ast->next){
        a=ast->next->number;
        if(ast->next->next){
            b=ast->next->next->number;
        }
    }
    if(a>=b){
        return newcell(serialctr++, "#t", 0, symbol);
    } else {
        return newcell(serialctr++, "#f", 0, symbol);
    }
}

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

cell *f_cond(cell *ast, environment *env){
    cell *test=NULL;
    while(ast->next)
    {
        ast=ast->next;
        if(ast->contents){
            if(ast->contents->contents){
                test=eval(ast->contents->contents, env);
            } else {
                test=eval(ast->contents, env);
            }
            if(ast->contents->next){
                if(!strcmp(test->symbol,"else") || !strcmp(test->symbol, "#t")){
                    return eval(ast->contents->next, env);
                }
            } else {
                return test;
            }
        }
    }    
    return ast;
}

cell *f_cons(cell *ast, environment *env){
    if(ast->next){
        cell *newast=copyCellDeep(ast->next);
        return newast;
    }
    return NULL;
}

cell *f_display(cell *ast, environment *env){
    if(ast->next){
        printf("%s", eval(ast->next, env)->symbol);
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

cell *f_read_char(cell *ast, environment *env){
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
        char *f=getStringfromAST(eval(ast->next, env));
        printf("%s", f);
    }
    return NULL;
}

cell *f_write_char(cell *ast, environment *env){
    if(ast->next){
        printf("%c", ast->next->symbol[0]);
    }
    return NULL;
}
