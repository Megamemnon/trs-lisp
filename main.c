#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <gc.h>

#define MAX_SYMBOL_LENGTH 50
#define INPUT_BUFFER_LENGTH 256

#pragma region Tokens

typedef enum TOKENTYPE{
    openparen, closeparen, tsymbol, tnumber, tstring, quote, eof
} tokentype;

typedef struct TOKEN{
    char symbol[MAX_SYMBOL_LENGTH];
    tokentype type;
}token;

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
        break;
    default:
        t.symbol[symbolix++]=c;
        t.type=tsymbol;
        while(!isspace(nc) && !isbreakingchar(nc)){
            c=getnxch();
            nc=peeknxch();
            t.symbol[symbolix++]=c;
        }
        t.symbol[symbolix]=0;
        break;
    }
    return t;
}

#pragma endregion Tokens

#pragma region AST

typedef enum VALTYPE{number, symbol, string, nil} valtype;

typedef struct CELL{
    long serial;
    char *symbol;
    float number;
    valtype type;
    struct CELL *contents;
    struct CELL *next;
} cell;

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
    newcl->contents=copyCellDeep(cl->contents);
    newcl->next=copyCellDeep(cl->next);
    return newcl;
}


char *getStringfromAST(cell *ast){
    if(!ast) return NULL;
    char *temp;
    char *contained;
    long contlen;
    char *next;
    long nexlen;
    if(ast->contents){
        temp=getStringfromAST(ast->contents);
        contlen=strlen(temp)+2;
        contained=GC_malloc(contlen+1);
        contained[0]='(';
        strcat(contained, temp);
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
    next=getStringfromAST(ast->next);
    if(next){
        contlen=strlen(contained);
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
        } else {
            return false;
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

typedef struct UNIFIER
{
    char *variable;
    cell *boundterm;
    struct UNIFIER *next;
}unifier;

unifier *newunifier(char *variable, cell *bound){
    unifier *u=(unifier *)GC_malloc(sizeof(unifier));
    u->variable=(char *)GC_malloc(strlen(variable)+1);
    strcpy(u->variable, variable);
    u->boundterm=copyCellDeep(bound);
    u->next=NULL;
    return u;
}

unifier *unify(cell *a, cell *b){
    unifier *c=NULL;
    unifier *n=NULL;
    if(!a->contents){
        if(a->type==symbol && isupper(a->symbol[0])){
            return newunifier(a->symbol, b);
        } else {
            if(!b->contents){
                if(b->type!=a->type || b->symbol!=a->symbol){
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
            while(x){
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

typedef struct RESOLUTION{
    unifier *unifier;
    cell *matchedcell;
    struct RESOLUTION *next;
}resolution;

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
    if(!ast->contents){
        if(!strcmp(ast->symbol, var)){
            if(!rplc->contents){
                strcpy(ast->symbol, rplc->symbol);
                ast->number=rplc->number;
                ast->type=rplc->type;
            } else {
                ast->contents=copyCellDeep(rplc);
                ast->type=nil;
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

typedef struct ENVIRONMENT{
    cell *bindings;
    cell *lastbinding;
    struct ENVIRONMENT *parent;
}environment;

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

#pragma region Primitives

typedef struct MACRO{
    char *name;
    cell *expression;
    cell *expansion;
    struct MACRO *next;
} macro;

macro *usermacros=NULL;


void addMacro(char *name, cell *expression, cell *expansion){
    macro *m=(macro *)GC_malloc(sizeof(macro));
    m->name=(char *)GC_malloc(strlen(name)+1);
    strcpy(m->name, name);
    m->expression=expression;
    m->expansion=expansion;
    m->next=NULL;
    if(usermacros){
        macro *x=usermacros;
        while(!x->next){
            x=x->next;
        }
        x->next=m;
    } else {
        usermacros=m;
    }
}

cell *f_cons(cell *ast, environment *env){
    if(ast->next){
        cell *newast=copyCellDeep(ast->next);
        return newast;
    }
    return NULL;
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

cell *f_display(cell *ast, environment *env){
    if(ast->next){
        printf("%s", ast->next->symbol);
    }
    return NULL;
}

cell *f_defun(cell *ast, environment *env){
    if(ast->next){
        if(ast->next->next) {
            cell *expression=copyCellDeep(ast->next);
            cell *expansion=copyCellDeep(ast->next->next);
            addMacro(ast->symbol, expression, expansion);
        }
    }
    return NULL;
}

typedef struct PRIMITIVE
{
    char *name;
    cell * (*f)(cell *, environment *);
    struct PRIMITIVE *next;
} primitive;

primitive *primitives=NULL;

primitive *newPrimitive(char *name, void *func){
    primitive *p=(primitive *)GC_malloc(sizeof(primitive));
    p->name=(char *)GC_malloc(strlen(name)+1);
    strcpy(p->name, name);
    p->f=func;
    return p;
}

void initPrimitives(){
    primitive *p=NULL;
    primitive *pn=NULL;
    p=newPrimitive("cons", f_cons);
    primitives=p;
    pn=p;
    p=newPrimitive("car", f_car);
    pn->next=p;
    p=p;
    p=newPrimitive("cdr", f_cdr);
    pn->next=p;
    p=p;
    p=newPrimitive("display", f_display);
    pn->next=p;
    p=p;
    p=newPrimitive("function", f_defun);
}

primitive *getPrimitive(char *name){
    primitive *p=primitives;
    while(p){
        if(!strcmp(p->name, name)){
            return p;
        }
        p=p->next;
    }
    return NULL;
}


// primitives prims[]={
// {"cons", f_cons},
// {"car", f_car},
// {"cdr", f_cdr},
// {"display", f_display},
// {"function", f_defun},
// {0}};

#pragma endregion

#pragma region Interpreter

cell *transform(cell *ast, cell *env){
    return ast;
}

cell *eval(cell *ast, environment *env){
    cell *x=NULL;
    environment *env2=newenvironment(env);
    if(!ast) return NULL;
    if(ast->next){
        ast->next=eval(ast->next, env2);
    }
    switch (ast->type)
    {
    case symbol:
        x=getVarBinding(ast->symbol, env);
        if(x){
            ast->symbol=x->symbol;
            ast->number=x->number;
            ast->contents=x->contents;
        } else {
            primitive *p=getPrimitive(ast->symbol);
            if(p){
                p->f(ast, env2);
            }  
        }
        break;
    case number:
        return ast;
        break;
    default:
        ast->contents=eval(ast->contents, env2);
        break;
    }
    return ast;
}

void interpret(environment *env){
    cell *cl=parse();
    char *formula=getStringfromAST(cl);
    printf("Parsing yields: %s\n",formula);
    eval(cl, env);
}


#pragma endregion Interpreter

#pragma region File and REPL interfaces

void loadfile(char *filename, environment *env){
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
            do{
                memfile[sz++]=fgetc(f);
            }while (c!=EOF);
            fclose(f);
        }
    }
    src=memfile;
    srcix=0;
    interpret(env);
}

void repl(environment *env){
    char in[INPUT_BUFFER_LENGTH];
    bool abort=false;
    while(!abort){
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
    printf("tr-lisp \nCopyright (c) 2023 Brian O'Dell\n");
    environment *env=newenvironment(NULL);
    repl(env);
    return 0;
}
