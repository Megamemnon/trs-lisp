
#include "primitives.h"

/* custom operators*/
cell *p_ansi_reset(cell *ast, environment *env){
    printf("\x1b[0m");
    return NULL;
}

cell *p_ansi(cell *ast, environment *env){
    int a=0;
    if(ast->next){
        a=eval(ast->next, env)->number;
        printf("\x1b[0;%dm", a);
    }
    return NULL;
}

cell *p_exit(cell *ast, environment *env){
    exit(0);
}

cell *p_pass(cell *ast, environment *env){
    return NULL;
}

cell *p_strcmp(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->next){
            cell *b=eval(ast->next->next, env);
            ast->next->next=NULL;
            cell *a = eval(ast->next, env);
            int x=strcmp(a->symbol, b->symbol);
            return newcell(serialctr++, NULL, x, number);
        }
    }
    return NULL;
}

cell *p_type(cell *ast, environment *env){
    if(ast->next){
        cell *e=eval(ast->next, env);
        switch (e->type)
        {
        case boolean:
            return newcell(serialctr++, "boolean", 0, string);
            break;
        case nil:
            return newcell(serialctr++, "nil", 0, string);
            break;
        case number:
            return newcell(serialctr++, "number", 0, string);
            break;
        case stream:
            return newcell(serialctr++, "stream", 0, string);
            break;
        case string:
            return newcell(serialctr++, "string", 0, string);
            break;
        case symbol:
            return newcell(serialctr++, "symbol", 0, string);
            break;
        
        default:
            break;
        }
    }
    return NULL;
}

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
        ast->next->next=NULL;
        a=eval(ast->next, env)->number;
    }
    if(a==b){
        return newcell(serialctr++, "#t", 0, boolean);
    } else {
        return newcell(serialctr++, "#f", 0, boolean);
    }
}

cell *f_lessthan(cell *ast, environment *env){
    float a, b;
    a=0;b=0;
    if(ast->next){
        if(ast->next->next){
            b=eval(ast->next->next, env)->number;
        }
        ast->next->next=NULL;
        cell *cl=eval(ast->next, env);
        if(cl->contents && !cl->next) cl=cl->contents;
        a=cl->number;
    }
    if(a<b){
        return newcell(serialctr++, "#t", 0, boolean);
    } else {
        return newcell(serialctr++, "#f", 0, boolean);
    }
}

cell *f_greaterthan(cell *ast, environment *env){
    float a, b;
    a=0;b=0;
    if(ast->next){
        if(ast->next->next){
            b=eval(ast->next->next, env)->number;
        }
        ast->next->next=NULL;
        cell *cl=eval(ast->next, env);
        if(cl->contents && !cl->next) cl=cl->contents;
        a=cl->number;
    }
    if(a>b){
        return newcell(serialctr++, "#t", 0, boolean);
    } else {
        return newcell(serialctr++, "#f", 0, boolean);
    }
}

cell *f_lessthanequal(cell *ast, environment *env){
    float a, b;
    a=0;b=0;
    if(ast->next){
        if(ast->next->next){
            b=eval(ast->next->next,env)->number;
        }
        ast->next->next=NULL;
        cell *cl=eval(ast->next, env);
        if(cl->contents && !cl->next) cl=cl->contents;
        a=cl->number;
    }
    if(a<=b){
        return newcell(serialctr++, "#t", 0, boolean);
    } else {
        return newcell(serialctr++, "#f", 0, boolean);
    }
}

cell *f_greaterthanequal(cell *ast, environment *env){
    float a, b;
    a=0;b=0;
    if(ast->next){
        if(ast->next->next){
            b=eval(ast->next->next, env)->number;
        }
        ast->next->next=NULL;
        cell *cl=eval(ast->next, env);
        if(cl->contents && !cl->next) cl=cl->contents;
        a=cl->number;
    }
    if(a>=b){
        return newcell(serialctr++, "#t", 0, boolean);
    } else {
        return newcell(serialctr++, "#f", 0, boolean);
    }
}

/* identifier operations */
cell *f_begin(cell *ast, environment *env){
    if(ast->next){
        return eval(ast->next, env);
    }
    return NULL;
}

cell *f_car(cell *ast, environment *env){
    if(ast->next){
        cell *cl=eval(ast->next, env);
        if(!cl->contents && cl->next){
            cell *a=newcell(serialctr++, NULL, 0, nil);
            a->contents=cl;
            ast->next=a;
        } else {
            ast->next=cl;
        }
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
        cell *cl=eval(ast->next, env);
        if(!cl->contents && cl->next){
            cell *a=newcell(serialctr++, NULL, 0, nil);
            a->contents=cl;
            ast->next=a;
        } else {
            ast->next=cl;
        }
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
    cell *cl=newcell(serialctr++, "#f", 0, boolean);
    if(ast->next){
        if(isalpha(eval(ast->next, env)->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_numeric(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, boolean);
    if(ast->next){
        if(isdigit(eval(ast->next, env)->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_whitespace(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, boolean);
    if(ast->next){
        if(isspace(eval(ast->next, env)->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_upper_case(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, boolean);
    if(ast->next){
        if(isupper(eval(ast->next, env)->symbol[0])){
            strcpy(cl->symbol,"#t");
            return cl;
        }
    }
    return cl;
}

cell *f_char_lower_case(cell *ast, environment *env){
    cell *cl=newcell(serialctr++, "#f", 0, boolean);
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

cell *f_close_input_port(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->type==stream){
            fclose(ast->next->stream);
        }
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
            if((a->type==symbol || a->type==string || a->type==boolean) && strcmp(a->symbol, b->symbol)) return false;
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
            cell *b=ast->next->next;
            ast->next->next=NULL;
            cell *a=ast->next;
            if(eqv(eval(a, env), eval(b, env))){
                return newcell(serialctr++, "#t", 0, boolean);
            }
        }
    }
    return newcell(serialctr++, "#f", 0, boolean);
}

cell *f_eq(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->next){
            if(ast->next->serial==ast->next->next->serial){
                return newcell(serialctr++, "#t", 0, boolean);
            }
        }
    }
    return newcell(serialctr++, "#f", 0, boolean);
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
            if(ast->next->type==symbol || ast->next->type==string){
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
        cell *cl=eval(ast->next, env);
        printf("%s", cl->symbol);
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

cell *f_eof_object(cell *ast, environment *env){
    if(ast->next){
        cell *cl=eval(ast->next, env);
        if(cl->symbol[0]==EOF){
            return newcell(serialctr++, "#t", 0, boolean);
        }
    }
    return newcell(serialctr++, "#f", 0, boolean);
}

cell *f_integer_to_char(cell *ast, environment *env){
    if(ast->next){
        int c=eval(ast->next, env)->number;
        char buff[2];
        buff[0]=(char )c;
        buff[1]=0;
        cell *cl=newcell(serialctr++, buff, c, string);
        return cl;
    }
    return NULL;
}

cell *f_let(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->next){
            cell *body=ast->next->next;
            cell *vars=ast->next->contents;
            environment *newenv=newenvironment(env);
            cell *v=vars;
            while(v){
                if(v->contents){
                    bindVar(v->contents->symbol, eval(v->contents->next, env), newenv);
                }
                v=v->next;
            }
            return eval(body, newenv);
        }
    }
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

cell *f_number_to_string(cell *ast, environment *env){
    if(ast->next){
        cell *cl=eval(ast->next, env);
        if(cl->type==number){
            char *buf=(char *)GC_malloc(MAX_SYMBOL_LENGTH);
            sprintf(buf, "%f", cl->number);
            return newcell(serialctr++, buf, cl->number, string);
        }
        return cl;
    }
    return NULL;
}

cell *f_open_input_file(cell *ast, environment *env){
    if(ast->next){
        char *filename=eval(ast->next, env)->symbol;
        FILE *f=fopen(filename, "r");
        cell *cl=newcell(serialctr++, filename, 0, stream);
        cl->stream=f;
        return cl;
    }
    return NULL;
}

cell *f_open_output_file(cell *ast, environment *env){
    if(ast->next){
        char *filename=eval(ast->next, env)->symbol;
        FILE *f=fopen(filename, "w");
        cell *cl=newcell(serialctr++, filename, 0, stream);
        cl->stream=f;
        return cl;
    }
    return NULL;
}

cell *f_quote(cell *ast, environment *env){
    return ast->next;
}

cell *f_read_char(cell *ast, environment *env){
    char *c=(char *)GC_malloc(2);
    c[1]=0;
    if(!ast->next){
        c[0]=getchar();
        cell *cl=newcell(serialctr++, c, c[0], string);
        return cl;
    } else {
        cell *st=eval(ast->next, env);
        if(st->type==stream){
            int ci=fgetc(st->stream);
            c[0]=(char )ci;
            return newcell(serialctr++, c, c[0], string);
        }
    }
    return NULL;
}

cell *f_set(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->next){
            cell *val=copyCellDeep(eval(ast->next->next, env));
            ast->next->next=NULL;
            cell *var=eval(ast->next, env);
            if(var->contents) var=var->contents;
            setVar(var->symbol, val, env);
        }
    }
    return NULL;
}

cell *f_string_append(cell *ast, environment *env){
    cell *s=ast->next;
    cell *cl=NULL;
    char *str1=NULL; char *str2=NULL;
    int strl=0; int morel=0;
    while(s){
        cl=copyCellDeep(s);
        cl->next=NULL;
        cl=eval(cl, env);
        if(cl->contents && !cl->next) cl=cl->contents;
        morel=strlen(cl->symbol);
        str2=(char *)GC_malloc(morel + strl + 1);
        if(strl>0){
            strcpy(str2, str1);
            strcat(str2, cl->symbol);
        } else {
            strcpy(str2, cl->symbol);
        }
        strl+=morel;
        str1=str2;
        s=s->next;
    }
    if(strl>0) return newcell(serialctr++, str1, 0, string);
    return NULL;
}

cell *f_string_eq(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->next){
            cell *b=eval(ast->next->next, env);
            ast->next->next=NULL;
            cell *a=eval(ast->next, env);
            if(!strcmp(a->symbol, b->symbol)){
                return newcell(serialctr++, "#t", 0, boolean);
            }
        }
    }
    return newcell(serialctr++, "#f", 0, boolean);
}

cell *f_string(cell *ast, environment *env){
    return f_string_append(ast, env);    
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

cell *f_substring(cell *ast, environment *env){
    int a, b;
    cell *cl=ast->next;
    if(ast->next){
        if(ast->next->next){
            if(ast->next->next->next){
                b=(int )eval(ast->next->next->next, env)->number-1;
            }
            ast->next->next->next=NULL;
            a=(int )eval(ast->next->next, env)->number-1;
            cl->next=NULL;
            cl=eval(cl, env);
            if(a<=b && b<=strlen(cl->symbol) && b>0){
                cl->symbol[b]=0;
                return newcell(serialctr++, cl->symbol+a, 0, string);
            }
        }
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
        if(!ast->next->next){
            printf("%c", eval(ast->next, env)->symbol[0]);
        } else {
            if(ast->next->next->stream){
                fputc(eval(ast->next, env)->symbol[0], ast->next->next->stream);
            }
        }        
    }
    return NULL;
}
