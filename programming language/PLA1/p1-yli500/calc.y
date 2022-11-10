%{
#include <stdio.h>
%}

%token TOK_SEMICOLON TOK_ADD TOK_SUB TOK_NUM TOK_PRINT

%union{
        float float_val;
}

%type <float_val> expr TOK_NUM

%left TOK_ADD TOK_SUB

%%

stmt: 
	| stmt expr_stmt
;

expr_stmt:
	   expr TOK_SEMICOLON
	   {
	   	yyerror();
	   }
	   | TOK_PRINT expr TOK_SEMICOLON 
		{
			fprintf(stdout, "%f\n", $2);
		} 
;

expr: 	 
	expr expr TOK_ADD
	  {
		$$ = $1 + $2;
	  }
	| expr expr TOK_SUB
	  {
		$$ = $1 - $2;
	  }
	| TOK_NUM
	  { 	
		$$ = $1;
	  }
;


%%

extern int yylineno;

int yyerror(char *s)
{
	printf("Parsing error: line %d\n", yylineno);
	return 0;
}

int main()
{
   yyparse();
   return 0;
}
