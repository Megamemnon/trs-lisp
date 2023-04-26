
#include "trs-lisp.h"
#include "primitives.h"

#pragma region Tokens

char *src;
long srcix=0;
long srcln=0;
long serialctr=0;
token pushedtoken;
bool pushedtokenavail=false;
char peeknxch(){return srcix>=srcln ? 0 : src[srcix];}
char getnxch(){return srcix>=srcln ? 0 : src[srcix++];}
bool isbreakingchar(char c){ return (c=='(' || c==')' || c=='\'') ? true : false;}

token getNextToken(){
    if(pushedtokenavail){
        pushedtokenavail=false;
        return pushedtoken;
    }
    token t;
    int symbolix=0;
    char c=getnxch();
    char nc=peeknxch();
    while(isspace(c)){
        c=getnxch();
        nc=peeknxch();
    }
    switch (c)
    {
    case 0:
        t.symbol[0]=0;
        t.type=eof;
        break;
    case '(':
        t.symbol[0]=c;
        t.symbol[1]=0;
        t.type=openparen;
        break;
    case ')':
        t.symbol[0]=c;
        t.symbol[1]=0;
        t.type=closeparen;
        break;
    case '\'':
        t.symbol[0]=c;
        t.symbol[1]=0;
        t.type=quote;
        break;
    case '"':
        c=getnxch();
        nc=peeknxch();
        t.type=tstring;
        while(c!='"'){
            if(!c){
                printf("Tokenization Error: End of quoted text not found: \"%s\".\n",t.symbol);
                t.type=eof;
                return t;
            }
            t.symbol[symbolix++]=c;
            c=getnxch();
            nc=peeknxch();
        }
        t.symbol[symbolix]=0;
        break;
    case '-': case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9': case '0':
        t.symbol[symbolix++]=c;
        t.type=tnumber;
        while(isdigit(nc) || nc=='.'){
            c=getnxch();
            nc=peeknxch();
            t.symbol[symbolix++]=c;
        }
        t.symbol[symbolix]=0;
        if(symbolix==1 && t.symbol[0]=='-'){
            t.type=tsymbol;
        }
        break;
    default:
        if(c=='#' && (nc=='t' || nc=='f')){
            t.symbol[0]=c;
            t.symbol[1]=nc;
            t.symbol[2]=0;
            t.type=boolean;
            c=getnxch();
        } else {
            t.symbol[symbolix++]=c;
            t.type=tsymbol;
            while(!isspace(nc) && !isbreakingchar(nc)){
                c=getnxch();
                nc=peeknxch();
                t.symbol[symbolix++]=c;
            }
            t.symbol[symbolix]=0;
        }
        break;
    }
    return t;
}

#pragma endregion Tokens

#pragma region AST

cell *newcell(long serial, char *symbol, float number, valtype type){
    cell *n=(cell *)GC_malloc(sizeof(cell));
    n->serial=serial;
    if(symbol){
        n->symbol=(char *)GC_malloc(strlen(symbol)+1);
        strcpy(n->symbol, symbol);
    } else {
        n->symbol=NULL;
    }
    n->number=number;
    n->type=type;
    n->contents=NULL;
    n->next=NULL;
    return n;
}

cell *copyCellDeep(cell *cl){
    if(!cl) return NULL;
    cell *newcl=newcell(cl->serial, cl->symbol, cl->number, cl->type);
    newcl->stream=cl->stream;
    newcl->contents=copyCellDeep(cl->contents);
    newcl->next=copyCellDeep(cl->next);
    return newcl;
}


char *getStringfromAST(cell *ast){
    if(!ast) return NULL;
    char *temp=NULL;
    char *contained = NULL;
    long contlen;
    char *next=NULL;
    long nexlen;
    if(ast->contents){
        temp=getStringfromAST(ast->contents);
        if(temp){
            contlen=strlen(temp)+2;
        } else {
            contlen=2;
        }
        contained=GC_malloc(contlen+1);
        contained[0]='(';
        if(temp) strcat(contained, temp);
        strcat(contained,")");
    }else{
        if(ast->type==string){
            contained=GC_malloc(strlen(ast->symbol)+3);
            contained[0]='"';
            strcat(contained,ast->symbol);
            strcat(contained, "\"");
        } else {
            contained=ast->symbol;
        }
    }
    if(ast->next){
        next=getStringfromAST(ast->next);
    }
    if(next){
        if(contained){
            contlen=strlen(contained);
        } else {
            return next;
        }
        nexlen=strlen(next);
        temp=(char *)GC_malloc(contlen+nexlen+2);
        strcpy(temp, contained);
        strcat(temp, " ");
        strcat(temp, next);
        return temp;
    }
    return contained;
}

bool equivalentCells(cell *a, cell *b){
    bool c=true;
    bool n=true;
    if(!a->contents){
        if(a->type==symbol && isupper(a->symbol[0])){
            return true;
        }
        if(!b->contents){
            if(a->type!=b->type){
                return false;
            }
            if(strcmp(a->symbol, b->symbol)){
                return false;
            }
        }
    } else {
        if(!b->contents){
            return false;
        }
        c=equivalentCells(a->contents, b->contents);
    }
    if(b->next){
        if(a->next){
            n=equivalentCells(a->next, b->next);
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

unifier *newunifier(char *variable, cell *bound){
    unifier *u=(unifier *)GC_malloc(sizeof(unifier));
    if(variable){
        u->variable=(char *)GC_malloc(strlen(variable)+1);
        strcpy(u->variable, variable);
    } else {
        u->variable=NULL;
    }
    u->boundterm=copyCellDeep(bound);
    //Only match S-Expressions, not lists
    u->boundterm->next=NULL;
    u->next=NULL;
    return u;
}

unifier *copyUnifierShallow(unifier *orig){
    unifier *u = newunifier(orig->variable, orig->boundterm);
    return u;
}

unifier *unify(cell *a, cell *b){
    unifier *c=NULL;
    unifier *n=NULL;
    if(!a->contents){
        if(a->type==symbol && isupper(a->symbol[0])){
            c=newunifier(a->symbol, b);
        } else {
            if(!b->contents){
                if(b->type!=a->type || strcmp(b->symbol,a->symbol)){
                    return NULL;
                } 
            } else {
                return NULL;
            }
        }
    } else {
        if(b->contents){
            c=unify(a->contents, b->contents);
        }
    }
    if(b->next){
        if(a->next){
            n=unify(a->next, b->next);
        } else {
            return NULL;
        }
    }
    if(!c && !n){
        return NULL;
    }
    if(c){
        if(n){
            unifier *x=c;
            while(x->next){
                x=x->next;
            }
            x->next=n;
            return c;
        } else {
            return c;
        }
    }
    return n;
}


resolution *newResolution(unifier *u, cell *mc){
    resolution *r=(resolution *)GC_malloc(sizeof(resolution));
    r->unifier=u;
    r->matchedcell=mc;
    r->next=NULL;
    return r;
}

resolution *resolve(cell *a, cell *b){
    resolution *r = NULL;
    resolution *r1=NULL;
    resolution *rn=NULL;
    unifier *u=NULL;
    if(equivalentCells(a, b)){
        u=unify(a, b);
        if(u){
            r=newResolution(u, b);
        }
    }
    if(b->contents){
        r1=resolve(a, b->contents);
        if(r1){
            if(!r){
                r=r1;
             } else {
                rn=r;
                while(rn->next){
                    rn=rn->next;
                }
                rn->next=r1;
             }
        }
    }
    if(b->next){
        r1=resolve(a, b->next);
        if(r1){
            if(!r){
                r=r1;
             } else {
                rn=r;
                while(rn->next){
                    rn=rn->next;
                }
                rn->next=r1;
             }
        }
    }
    return r;
}

void replaceNode(cell *expr, cell *rplc, cell *ast){
    if(ast->contents){
        if(ast->contents->serial==expr->serial){
            ast->contents=rplc;
        } else {
            replaceNode(expr, rplc, ast->contents);
        }
    }
    if(ast->next){
        if(ast->next->serial==expr->serial){
            ast->next=rplc;
        } else {
            replaceNode(expr, rplc, ast->next);
        }
    }
}

void replaceVariable(char *var, cell *rplc, cell *ast){
    if(!ast) return;
    if(!ast->contents && ast->symbol){
        if(!strcmp(ast->symbol, var)){
            if(!rplc->contents){
                ast->symbol=(char *) GC_malloc(strlen(rplc->symbol)+1);
                strcpy(ast->symbol, rplc->symbol);
                ast->number=rplc->number;
                ast->type=rplc->type;
            } else {
                ast->symbol=NULL;
                ast->type=nil;
                ast->number=0;
                ast->contents=rplc->contents;
            }
        }
    } else {
        replaceVariable(var, rplc, ast->contents);
    }
    if(ast->next){
        replaceVariable(var, rplc, ast->next);
    }
}

void applyUnifier(cell *ast, unifier *u){
    unifier *un=u;
    while(un){
        if(!ast->contents){
            if(!strcmp(ast->symbol,u->variable)){
                ast->contents=copyCellDeep(u->boundterm);
                ast->type=nil;
            } else {
                replaceVariable(u->variable,u->boundterm, ast);
            }
        } else {
            replaceVariable(u->variable, u->boundterm, ast);
        }
        un=un->next;
    }
}

#pragma endregion AST

#pragma region Parser

cell *parse(){
    token t=getNextToken();
    cell *head=NULL;
    cell *tail=NULL;
    cell *node=NULL;
    while(t.type!=eof){
        switch (t.type)
        {
        case openparen:
            node=newcell(serialctr++, NULL, 0, nil);
            node->contents=parse();
            break;
        case closeparen:
            // pushedtoken=t;
            // pushedtokenavail=true;
            return head;
            break;
        case tnumber:
            node=newcell(serialctr++, t.symbol, strtof(t.symbol, NULL), number);
            break;
        case tstring:
            node=newcell(serialctr++, t.symbol, 0, string);
            break;
        case tboolean:
            node=newcell(serialctr++, t.symbol, 0, boolean);
            break;
        default:
            node=newcell(serialctr++, t.symbol, 0, symbol);
            break;
        }
        if(node!=NULL){
            if(head==NULL){
                head=node;
                tail=node;
            }else{
                tail->next=node;
                tail=tail->next;
            }
        }
        t=getNextToken();
    }
    return head;
}

#pragma endregion Parser

#pragma region Environment


environment *newenvironment(environment *parent){
    environment *n=(environment *)GC_malloc(sizeof(environment));
    n->parent=parent;
    n->bindings=NULL;
    n->lastbinding=NULL;
    return n;
}

void bindVar(char *var, cell *bound, environment *env){
    cell *binding=newcell(serialctr++, var, 0, symbol);
    binding->contents=bound;
    if(env->lastbinding==NULL){
        env->bindings=binding;
        env->lastbinding=binding;
    } else {
        env->lastbinding->next=binding;
        env->lastbinding=binding;
    }
}

void setVar(char *var, cell *bound, environment *env){
    cell *binding=env->bindings;
    while(binding){
        if(!strcmp(var, binding->symbol)){
            binding->contents=bound;
            return;
        }
        binding=binding->next;
    }
}

cell *getVarBinding(char *var, environment *env){
    cell *b=env->bindings;
    environment *e=env;
    while(e){
        while(b){
            if(!strcmp(var, b->symbol)){
                return b->contents;
            }
            b=b->next;
        }
        e=e->parent;
    }
    return NULL;
}

#pragma endregion Environment

#pragma region Macros

macro *macros=NULL;
macro *functions=NULL;

macro *newMacro(char *name, cell *expression, cell *expansion){
    macro *m=(macro *)GC_malloc(sizeof(macro));
    m->name=(char *)GC_malloc(strlen(name)+1);
    strcpy(m->name, name);
    m->expression=expression;
    m->expansion=expansion;
    m->next=NULL;
    return m;
}

void addMacro(macro *m){
    if(macros){
        macro *x=macros;
        while(x->next){
            x=x->next;
        }
        x->next=m;
    } else {
        macros=m;
    }
}

void addFunction(macro *m){
    if(functions){
        macro *x=functions;
        while(x->next){
            x=x->next;
        }
        x->next=m;
    } else {
        functions=m;
    }
}

#pragma endregion Macros

#pragma region Primitives

primitive *primitives=NULL;
// primitive *primitiveops=NULL;
// primitive *primitivefuncs=NULL;

primitive *newPrimitive(char *name, void *func){
    primitive *p=(primitive *)GC_malloc(sizeof(primitive));
    p->name=(char *)GC_malloc(strlen(name)+1);
    strcpy(p->name, name);
    p->f=func;
    return p;
}


#define addprim(sym,func)   p->next=newPrimitive(sym, func); p=p->next;

void initPrimitives(){
    primitive *p=NULL;
    primitives=newPrimitive("+", f_add);
    p=primitives;
    addprim("-",f_sub)
    addprim("*",f_mul)
    addprim("/",f_div)
    addprim("=",f_equalsign)
    addprim("<",f_lessthan)
    addprim(">",f_greaterthan)
    addprim("<=",f_lessthanequal)
    addprim(">=",f_greaterthanequal)
#ifdef R5RS_PRIMITIVES
    addprim("begin", f_begin)
    addprim("car",f_car)
    addprim("cdr",f_cdr)
    addprim("char->integer",f_char_to_integer)
    addprim("char-alphabetic?",f_char_alphabetic)
    addprim("char-numeric?",f_char_numeric)
    addprim("char-whitespace?",f_char_whitespace)
    addprim("char-upper-case?",f_char_upper_case)
    addprim("char-lower-case?",f_char_lower_case)
    addprim("close-input-port", f_close_input_port)
    addprim("close-output-port", f_close_input_port)
    addprim("cond",f_cond)
    addprim("cons",f_cons)
    addprim("define",f_define)
    addprim("define-syntax",f_define_syntax)
    addprim("display",f_display)
    addprim("do",f_do)
    addprim("eof-object?", f_eof_object)
    addprim("eqv?", f_eqv)
    addprim("eq?", f_eqv)
    addprim("integer->char",f_integer_to_char)
    addprim("integer->string", f_integer_to_string)
    addprim("let",f_let)
    addprim("list", f_list)
    addprim("load",f_load)
    addprim("newline",f_newline)
    addprim("number->string", f_number_to_string)
    addprim("open-input-file", f_open_input_file)
    addprim("open-output-file", f_open_output_file)
    addprim("'", f_quote)
    addprim("quote", f_quote)
    addprim("read-char",f_read_char)
    addprim("set!", f_set)
    addprim("string->symbol", f_string_to_symbol)
    addprim("string=?", f_string_eq)
    addprim("string-append", f_string_append)
    addprim("string",f_string)
    addprim("string-length",f_string_length)
    addprim("substring", f_substring)
    addprim("write",f_write)
    addprim("write-char",f_write_char)

    addprim("ansi-code", p_ansi_code)
    addprim("ansi-reset", p_ansi_reset)
    addprim("define-function", p_define_function)
    addprim("exit", p_exit)
    addprim("noop", p_pass)
    addprim("strcmp", p_strcmp)
    addprim("type", p_type)
#else

#endif

}

primitive *getPrimitive(char *name){
    primitive *p=primitives;
    while(p){
        if(!strcmp(p->name, name)) return p;
        p=p->next;
    }
    return NULL;
}

macro *getFunction(char *name){
    macro *f=functions;
    while(f){
        if(!strcmp(f->name, name)) return f;
        f=f->next;
    }
    return NULL;
}


#pragma endregion

#pragma region Interpreter

cell *applyMacros(cell *ast, environment *env){
    cell *cl=ast;
    char *before=NULL;
    char *after=NULL;
    if(cl){
        bool changed=true;
        while(changed){
            changed=false;
            before=getStringfromAST(cl);
            macro *m=macros;
            while(m){
                resolution *res=resolve(m->expression, cl);
                resolution *r=res;
                while (r){
                    cell *expansion=copyCellDeep(m->expansion);
                    unifier *u=r->unifier;
                    if(u){
                        while(u){
                            unifier *u0=copyUnifierShallow(u);
                            u0->next=NULL;
                            applyUnifier(expansion, u0);
                            u=u->next;
                        }
                    }
                    changed=true;
                    if(r->matchedcell->serial==cl->serial){
                        cl=expansion;
                    } else {
                        replaceNode(r->matchedcell, expansion, cl);
                    }
                    r=r->next;
                }
                m=m->next;
            }
            if(changed){
                after=getStringfromAST(cl);
                if(!strcmp(before, after)){
                    changed=false;
                }
            }
        }
    }
    return cl;
}


cell *applyFunctions(cell *ast, environment *env){
    cell *cl=ast;
    char *before=NULL;
    char *after=NULL;
    if(cl){
        bool changed=true;
        while(changed){
            changed=false;
            before=getStringfromAST(cl);
            macro *m=functions;
            while(m){
                resolution *res=resolve(m->expression, cl);
                resolution *r=res;
                while (r){
                    cell *expansion=copyCellDeep(m->expansion);
                    unifier *u=r->unifier;
                    if(u){
                        while(u){
                            unifier *u0=copyUnifierShallow(u);
                            u0->next=NULL;
                            applyUnifier(expansion, u0);
                            u=u->next;
                        }
                    }
                    changed=true;
                    if(r->matchedcell->serial==cl->serial){
                        cl=expansion;
                    } else {
                        replaceNode(r->matchedcell, expansion, cl);
                    }
                    r=r->next;
                }
                m=m->next;
            }
            if(changed){
                after=getStringfromAST(cl);
                if(!strcmp(before, after)){
                    changed=false;
                }
            }
        }
    }
    return cl;
}


cell *eval(cell *ast, environment *env){
    cell *x=NULL;
    if(!ast) return NULL;
    switch (ast->type)
    {
    case symbol:
        x=copyCellDeep(getVarBinding(ast->symbol, env));
        if(x){
            if(!x->next){
                x->next=ast->next;
                ast=x;
            } else {
                if(!ast->next){
                    ast=x;
                } else {
                    ast->contents=x;
                }
            }
        } else {
            primitive *p=getPrimitive(ast->symbol);
            if(p){
                cell *opresult= p->f(ast, env);
                return opresult;
            } else {
                if(ast->next){
                    macro *f=getFunction(ast->symbol);
                    if(f){
                        // cell *cl=applyFunctions(ast, env);
                        // return eval(cl, env);
                        cell *var=f->expression->next;
                        cell *val=ast->next;
                        cell *exp=copyCellDeep(f->expansion);
                        while(var){
                            if(!val){
                                printf("Missing required parameters in call to '%s'.\n", f->name);
                                exit(EXIT_FAILURE);
                            }
                            cell *valcopy=copyCellDeep(val);
                            valcopy->next=NULL;
                            replaceVariable(var->symbol, valcopy, exp);
                            var=var->next;
                            val=val->next;
                        }
                        return eval(exp, env);
                    }
                }
            }
        }
        break;
    case number: case string: case boolean:
        break;
    default:
        cell *contentsresult=eval(ast->contents, env);
        if(!ast->next){
            return contentsresult;
        } else {
            if(contentsresult){
                if(contentsresult->contents && !contentsresult->next){
                    ast->contents=contentsresult->contents;
                } else {
                    ast->contents=contentsresult;
                }
            } else {
                ast->contents=NULL;
            }
        }
        break;
    }
    if(ast->next){
        cell *nextresult=eval(ast->next, env);
        if(nextresult){
            if(nextresult->contents && !nextresult->next){
                ast->next=nextresult->contents;
            } else {
                ast->next=nextresult;
            }
        } else {
            ast->next=NULL;
        }
    }
    return ast;
}

void interpret(environment *env){
    cell *cl=parse();
    char *formula=NULL;
#ifdef DEBUG_VERBOSE
    formula=getStringfromAST(cl);
    printf("Parsing yields:\n  %s\n",formula);
#endif
    cell *ast=applyMacros(cl, env);
#ifdef DEBUG_VERBOSE
    formula=getStringfromAST(ast);
    printf("Applying Macros yields:\n  %s\n",formula);
#endif
    cell *r=eval(ast, env);
    if(!r){
        // printf("\nnil");
        return;
    }
    if(r->type!=nil || r->contents || r->next){
        formula=getStringfromAST(r);
        if(formula) printf("\n%s\n",formula);
    }
}

#pragma endregion Interpreter

#pragma region File and REPL interfaces

void loadfile(const char *filename, environment *env){
    FILE *f=fopen(filename, "r");
    char c;
    long sz=0;
    char *memfile;
    if(f){
        do{
            c=fgetc(f);
            sz++;
        } while(c!=EOF);
        memfile=GC_malloc(sz+1);
        srcln=sz;
        fclose(f);
        f=fopen(filename, "r");
        if(f){
            sz=0;
            int paren=0;
            bool hasexpr=false;
            do{
                c=fgetc(f);
                if(c=='('){
                    paren++;
                    hasexpr=true;
                }
                if(c==')') paren--;
                if(c==EOF){
                    memfile[sz++]=0;
                }else{
                    memfile[sz++]=c;
                    if(paren==0 && hasexpr){
                        memfile[sz]=0;
                        src=memfile;
                        srcix=0;
                        interpret(env);
                        sz=0;
                        hasexpr=false;
                    }
                }
            }while (c!=EOF);
            fclose(f);
        }
    } else {
        printf("\nUnable to load %s", filename);
    }
}

void repl(environment *env){
    char in[INPUT_BUFFER_LENGTH];
    bool abort=false;
    while(!abort){
        printf("\n> ");
        fgets(in, INPUT_BUFFER_LENGTH, stdin);
        if(strcmp(in, "(exit)\n")){
            srcln=strlen(in);
            src=(char *)GC_malloc(srcln+1);
            strcpy(src, in);
            srcix=0;
            interpret(env);
        }else{
            abort=true;
        }
    }
}

#pragma endregion File and REPL

int main(int argc, char const *argv[])
{
    GC_init();
    initPrimitives();
    printf("trs-lisp \nCopyright (c) 2023 Brian O'Dell\n");
    environment *env=newenvironment(NULL);
#ifdef DEBUG
    loadfile(DEBUG_FILE_1, env);
    loadfile(DEBUG_FILE_2, env);
#else    
    int argcounter=1;
    while(argcounter<argc){
        int arglen=strlen(argv[argcounter]);
        if(arglen>5){
            const char *lastn=&argv[argcounter][arglen-5];
            if(!strcmp(lastn, ".trsl")){
                loadfile(argv[argcounter], env);
            }
        }
        argcounter++;
    }
#endif
    repl(env);
    return 0;
}
