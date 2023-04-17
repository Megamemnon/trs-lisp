
#include "primitives.h"

/* symbol operators */

cell *f_add(cell *ast, environment *env){
    float a, b;
    a=0;b=0;
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
    a=0;b=0;
    if(ast->next){
        if(ast->next->next){
            b=eval(ast->next->next, env)->number;
        }
        a=eval(ast->next, env)->number;
    }
    char buffer[20];
    int n=sprintf(buffer, "%f", a-b);
    return newcell(serialctr++, buffer, a-b, number);
}

cell *f_mul(cell *ast, environment *env){
    float a, b;
    a=1;b=1;
    if(ast->next){
        if(ast->next->next){
            b=eval(ast->next->next, env)->number;
        }
        a=eval(ast->next, env)->number;
    }
    char buffer[20];
    int n=sprintf(buffer, "%f", a*b);
    return newcell(serialctr++, buffer, a*b, number);
}

cell *f_div(cell *ast, environment *env){
    float a, b;
    a=1;b=1;
    if(ast->next){
        if(ast->next->next){
            b=eval(ast->next->next, env)->number;
        }
        a=eval(ast->next, env)->number;
    }
    char buffer[20];
    int n=sprintf(buffer, "%f", a/b);
    return newcell(serialctr++, buffer, a/b, number);
}

cell *f_equalsign(cell *ast, environment *env){
    float a, b;
    a=0;b=0;
    if(ast->next){
        if(ast->next->next){
            b=eval(ast->next->next, env)->number;
        }
        a=eval(ast->next, env)->number;
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
        if(ast->next->next){
            b=eval(ast->next->next, env)->number;
        }
        a=eval(ast->next, env)->number;
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
        if(ast->next->next){
            b=eval(ast->next->next, env)->number;
        }
        a=eval(ast->next, env)->number;
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
        if(ast->next->next){
            b=eval(ast->next->next,env)->number;
        }
        a=eval(ast->next, env)->number;
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
        if(ast->next->next){
            b=eval(ast->next->next, env)->number;
        }
        a=eval(ast->next, env)->number;
    }
    if(a>=b){
        return newcell(serialctr++, "#t", 0, symbol);
    } else {
        return newcell(serialctr++, "#f", 0, symbol);
    }
}

/* identifier operations */

cell *f_car(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->contents){
            cell *car=eval(ast->next->contents, env);
            car->next=NULL;
            car=eval(car, env);
            return car;
        }
    }
    return NULL;
}

cell *f_cdr(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->contents) {
            cell *cdr=eval(ast->next->contents, env);
            if(cdr->next){
                cdr=cdr->next;
                cdr=eval(cdr, env);
                return cdr;
            }
        }
    }
    return NULL;
}

cell *f_char_alphabetic(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(isalpha(eval(ast->next, env)->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_numeric(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(isdigit(eval(ast->next, env)->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_whitespace(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(isspace(eval(ast->next, env)->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_upper_case(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(isupper(eval(ast->next, env)->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_lower_case(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(islower(eval(ast->next, env)->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}


cell *f_char_to_integer(cell *ast, environment *env){
    if(ast->next){
        char c=eval(ast->next, env)->symbol[0];
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
                test=copyCellDeep(ast->contents->contents);
                test=eval(test, env);
            } else {
                test=copyCellDeep(ast->contents);
                test->next=NULL;
                test=eval(test, env);
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
    cell *cons=newcell(serialctr, NULL, 0, nil);
    cons->contents=eval(ast->next, env);
    return cons;
}

bool eqv(cell *a, cell *b){
    bool c=true;
    bool n=true;
    if(!a->contents){
        if(!b->contents){
            if(a->type!=b->type) return false;
            if((a->type==symbol || a->type==string ) && strcmp(a->symbol, b->symbol)) return false;
            if(a->type==number && a->number!=b->number) return false;
        }
    } else {
        if(!b->contents) return false;
        c=eqv(a->contents, b->contents);
    }
    if(b->next){
        if(a->next){
            n=eqv(a->next, b->next);
        } else {
            return false;
        }
    } else {
        if(a->next){
            return false;
        }
    }
    return c && n;
}

cell *f_eqv(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->next){
            if(eqv(eval(ast->next, env), eval(ast->next->next, env))){
                return newcell(serialctr++, "#t", 0, symbol);
            }
        }
    }
    return newcell(serialctr++, "#f", 0, symbol);
}

cell *f_eq(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->next){
            if(ast->next->serial==ast->next->next->serial){
                return newcell(serialctr++, "#t", 0, symbol);
            }
        }
    }
    return newcell(serialctr++, "#f", 0, symbol);
}

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
                    cell *n=copyCellDeep(ast->next);
                    n->next=NULL;
                    bindVar(eval(n, env)->symbol, eval(ast->next->next, env), env);
                }
            }
        }
    }
    return NULL;
}

cell *f_define_syntax(cell *ast, environment *env){
    return NULL;
}

cell *f_display(cell *ast, environment *env){
    if(ast->next){
        printf("%s", eval(ast->next, env)->symbol);
    }
    return NULL;
}

cell *f_do(cell *ast, environment *env){
    cell *vars=NULL;
    cell *v=NULL;
    cell *step=NULL;
    cell *test=NULL;
    cell *testresult=NULL;
    cell *iftrue=NULL;
    cell *command=NULL;
    if(ast->next){
        if(ast->next->contents){
                vars=ast->next->contents;
            if(ast->next->next){
                if(ast->next->next->contents){
                    test=copyCellDeep(ast->next->next->contents);
                    iftrue=test->next;
                    test->next=NULL;     
                }
            }
            if(ast->next->next->next){
                command=ast->next->next->next;
            }
        }
    }
    if(vars && test && command){
        // setup variables
        v=vars;
        while(v){
            if(v->contents){
                cell *init=copyCellDeep(v->contents->next);
                init->next=NULL;
                bindVar(v->contents->symbol, eval(init, env), env);
            }
            v=v->next;
        }
        // loop
        while(1){
            // test
            testresult=eval(test, env);
            if(testresult->contents && !testresult->next) testresult=testresult->contents;
            if(!strcmp(testresult->symbol, "#t")){
                return eval(iftrue, env);
            } else {
                eval(copyCellDeep(command), env);
            }
            // step
            v=vars;
            while(v){
                if(v->contents){
                    if(v->contents->next){
                        if(v->contents->next->next){
                            step=copyCellDeep(v->contents->next->next);
                            setVar(v->contents->symbol,eval(step, env), env);
                        }
                    }
                }
                v=v->next;
            }
        }
    }
    return NULL;
}

cell *f_integer_to_char(cell *ast, environment *env){
    if(ast->next){
        char *c=(char *)GC_malloc(2);
        c[0]=eval(ast->next, env)->number;
        cell *cl=newcell(serialctr++, c, 0, string);
        return cl;
    }
    return NULL;
}

cell *f_let(cell *ast, environment *env){
    return NULL;
}

cell *f_list(cell *ast, environment *env){
    cell *list=newcell(serialctr++, NULL, 0, nil);
    list->contents=eval(ast->next, env);
    return list;
}

cell *f_load(cell *ast, environment *env){
    if(ast->next){
        if(ast->type==string || ast->type==symbol){
            loadfile(ast->symbol, env);
        }
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

cell *f_set(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->next){
            setVar(eval(ast->next, env)->symbol, eval(ast->next->next, env), env);
        }
    }
    return NULL;
}

cell *f_string(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, symbol);
    if(ast->next){
        if(eval(ast->next, env)->type==string){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_string_length(cell *ast, environment *env){
    if(ast->next){
        int l=strlen(eval(ast->next, env)->symbol);
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
        printf("%c", eval(ast->next, env)->symbol[0]);
    }
    return NULL;
}
