%{
    #include "node.h"

    void yyerror(char *);
    int yylex(void);
    TNode * sym[26];
%}

%union
{
    TNode *node;
    TVariable *var_node;
    TNumber *num_node;
};

%token <num_node> INTEGER
%token <var_node> VARIABLE

%type <node> expr

%left '+' '-'
%left '*' '/'

%%

program:
    program statement '\n'
    |
    ;

statement:
    expr { $1->Print(); printf("\n"); }
    | VARIABLE '=' expr { sym[$1->Name - 'a'] = $3; }
    ;

expr:
    INTEGER
    | VARIABLE { $$ = sym[$1->Name - 'a']; }
    | expr '+' expr {
        TOperation * res = new TOperation('+');
        res->Left = $1;
        res->Right = $3;
        $$ = res; }
    | expr '-' expr {
        TOperation * res = new TOperation('-');
        res->Left = $1;
        res->Right = $3;
        $$ = res; }
    | expr '*' expr {
        TOperation * res = new TOperation('*');
        res->Left = $1;
        res->Right = $3;
        $$ = res; }
    | expr '/' expr {
        TOperation * res = new TOperation('/');
        res->Left = $1;
        res->Right = $3;
        $$ = res; }
    | '(' expr ')' { $$ = $2; }
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}
