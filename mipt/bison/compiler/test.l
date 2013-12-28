%{

#include "Module.hpp"

#include "test.tab.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

%}

%option noyywrap
%option c++

DIGIT [0-9]
LETTER [a-zA-Z]+
ID    {LETTER}({LETTER}|{DIGIT})*
DOUBLE ({DIGIT}+)"."({DIGIT}+)
BOOLEAN "TRUE"|"true"|"True"|"FALSE"|"False"|"false"
STRING "\""[^\"]*"\""
BLANK [ \t\n]

%%

":="        { return ASSGNOP;     }
"<="        { return LE;          }
">="        { return GE;          }
"<>"        { return NE;          }
DIV         { return DIV;         }
MOD         { return MOD;         }
OR          { return OR;          }
{DIGIT}+    { 
              yylval.integer = new IntegerValue(atoi(yytext));
              return INTEGER;     
            }
{BOOLEAN}   { 
							if (tolower(*yytext) == 't')
              	yylval.boolean = new BooleanValue(1);
							if (tolower(*yytext) == 'f')
              	yylval.boolean = new BooleanValue(0);
              return BOOLEAN;     
            }
{STRING}    { 
							std::string s(yytext);
            	yylval.string = new StringValue(s.substr(1, s.length() - 2));
            	return STRING;
            }
{DOUBLE}    { 
            	yylval.double_ = new DoubleValue(atof(yytext));
            	return DOUBLE;
            }
MODULE      { return MODULE;      }
PROCEDURE   { return PROCEDURE;   }
BEGIN       { return BEGIN_;       }
END         { return END;         }
CONST       { return CONST;       }
TYPE        { return TYPE;        }
VAR         { return VAR;         }
RECORD      { return RECORD;      }
ARRAY       { return ARRAY;       }
OF          { return OF;          }
WHILE       { return WHILE;       }
DO          { return DO;          }
ELSE        { return ELSE;        }
IF          { return IF;          }
ELSIF       { return ELSIF;       }
THEN        { return THEN;        }
{ID}        { 
              yylval.identifier = new std::string(yytext, yyleng);
              return IDENTIFIER;  
            }
{BLANK}+    /* blank, tab, newline */
.           { return yytext[0];   }

%%
