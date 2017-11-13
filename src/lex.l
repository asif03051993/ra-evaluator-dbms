%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
extern "C" int yylex();
%}

number				[0-9]+|[0-9]+[.][0-9]+
identifier			[a-zA-Z][a-zA-Z|0-9|_]* 
ident				[a-zA-Z][a-zA-Z|0-9|_|*|+|-|/|(|)]* 
escape				[ \t]+

%%

pi|PI               {return PI;}
sigma|SIGMA         {return SIGMA;}
"-"                 {return DIFFERENCE;}
"X"    	            {return PRODUCT;}
"U"    	            {return UNION;}
"rho"    	    {return RENAME;}
")"                 {return CRB;}
"("                 {return CLB;}
"}"                 {return RB;}
"{"                 {return LB;}
"!="                {return NE;}
"^"                 {return AND;}
"|"                 {return OR;}
"!"                 {return NOT;}
">="                {return GE;}
"<="                {return LE;}
"="                 {return EQ;}
"<"                 {return LT;}
">"                 {return GT;}
"["                 {return SLB;}
"]"                 {return SRB;}
","                 {return COMMA;}
"\""                 {return QUOTE;}
{number}	    {strcpy(yylval.number,yytext);return NUMBER;}
{identifier}        {strcpy(yylval.string,yytext);return STRING;}
{ident}             {strcpy(yylval.string,yytext);return STRING;}
{escape}	    {}
"\n"		    {}
.		    {}

%%
int yywrap(void) {
	return 1;
}
