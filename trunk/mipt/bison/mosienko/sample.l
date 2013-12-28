%{
    #include <stdlib.h>
    #include "node.h"
    #include "sample.tab.h"

    void yyerror(char *);
%}

%option noyywrap

%%

[a-z]         { yylval.var_node = new TVariable(*yytext); return VARIABLE; }

[0-9]+        { yylval.num_node = new TNumber(atoi(yytext)); return INTEGER; }

[-+()=/*\n]   { return *yytext; }

[ \t]         ;

.             { yyerror("invalid character"); }

%%
