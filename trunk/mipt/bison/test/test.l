%{

#include "VariableType.hpp"
#include "Selector.hpp"
#include "test.tab.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

%}

%option noyywrap
%option c++

DIGIT [0-9]
LETTER [a-zA-Z]
ID    {LETTER}+({LETTER}+{DIGIT})*
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
              yylval.integer = new Integer(atoi(yytext));
              return INTEGER;     
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
              yylval.id = new std::string(yytext, yyleng);
              return IDENTIFIER;  
            }
{BLANK}+    /* blank, tab, newline */
.           { return yytext[0];   }

%%
