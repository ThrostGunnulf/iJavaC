%{
#include <stdio.h>

void yyerror(char *s);

extern int yylineno;
extern int prevColNo;
extern char *yytext;
%}

%token INT BOOL NEW IF ELSE WHILE PRINT PARSEINT CLASS PUBLIC STATIC VOID STRING DOTLENGTH RETURN AND OR RELCOMPAR BOOLLIT ID INTLIT RESERVED EQUALITY ADDITIVE MULTIPLIC

%left OR
%left AND
%left EQUALITY
%left RELCOMPAR
%left ADDITIVE
%left MULTIPLIC
%right UNARY

%nonassoc EXPR1REDUCE

%left '[' DOTLENGTH

%nonassoc IFX
%nonassoc ELSE

%%

start: CLASS ID '{' decls '}'                                                    {}
	 | CLASS ID '{' '}'                                     		             {}

decls: decls fielddecl                                                           {}
     | decls methoddecl                                                          {}
     | fielddecl                                                                 {}
     | methoddecl                                                                {};

fielddecl: STATIC vardecl                                                        {};

methoddecl: PUBLIC STATIC methodtype ID '(' formalparams ')' '{' vardecl stmtlist '}'    {};

methodtype: type                                                                 {}
          | VOID                                                                 {};

formalparams: type ID formalparamslist
            | STRING '[' ']' ID                                                  {}
            | ;

formalparamslist: formalparamslist ',' type ID                                   {}
                | ;

stmtlist: stmtlist statement                                                     {}
        | ;

vardecl: type ID vardecllist ';'                                                 {};

vardecllist: vardecllist ',' ID                                                  {}
           | ;

type: INT '[' ']'                                                                {}
    | BOOL '[' ']'                                                               {}
    | INT                                                                        {}
    | BOOL                                                                       {};

statement: '{' stmtlist '}'                                                      {}
         | IF '(' expr ')' statement ELSE statement   %prec ELSE                 {}
         | IF '(' expr ')' statement                  %prec IFX                  {}
         | WHILE '(' expr ')' statement                                          {}
         | PRINT '(' expr ')' ';'                                                {}
         | ID '[' expr ']' '=' expr ';'                                          {}
         | ID '=' expr ';'                                                       {}
         | RETURN expr ';'                                                       {}
         | RETURN ';'                                                            {};

expr: expr1				  %prec EXPR1REDUCE									      {}
	| expr1 '[' expr ']'  				                         				  {}
	| expr2																		  {};

expr1: expr AND expr                                                              {}
     | expr OR expr                                                               {}
     | expr RELCOMPAR expr                                                        {}
     | expr EQUALITY expr                                                         {}
     | expr ADDITIVE expr                                                         {}
     | expr MULTIPLIC expr                                                        {}
     | ID                                                                         {}
     | INTLIT                                                                     {}
     | BOOLLIT                                                                    {}
     | '(' expr ')'                                                               {}
     | expr DOTLENGTH                                                             {}
     | '!' expr          %prec UNARY                                              {}
     | ADDITIVE expr     %prec UNARY                                              {}
     | PARSEINT '(' ID '[' expr ']' ')'                                           {}
     | ID '(' args ')'                                                            {}
     | ID '(' ')'                                                                 {};

expr2: NEW INT '[' expr ']'                                                       {}
     | NEW BOOL '[' expr ']'                                                      {};

args: expr argslist                                                               {}
    | expr                                                                        {};
 
argslist: ',' args                                                                {};

%%

int main()
{
	yyparse();
    return 0;
}

void yyerror(char *s) {printf("Line %d, col %d: %s: %s\n", yylineno, prevColNo, s, yytext);}
