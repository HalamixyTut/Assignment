%{
#include <stdio.h>
#include "calc.tab.h"
%}

digit	([0-9])

%option yylineno

%%

"print"       {       return(TOK_PRINT);}


{digit}+ {
		sscanf(yytext, "%f", &yylval);
		return TOK_NUM;
}

{digit}+"."{digit}+ {
	sscanf(yytext, "%f", &yylval);                   
		return TOK_NUM;
}

";"	{	return(TOK_SEMICOLON);	}

"+"	{	return(TOK_ADD);	}
"-"	{	return(TOK_SUB);	}
" " {}

[\t] {}

[ \n] 	{}

.	{printf("Lexical analysis error: %c\n", 
			yytext[0]);
	}

%%

