%{
#include <stdio.h>
//#include "astNodes.h"

extern int prevLineNo;
extern int prevColNo;
extern char *yytext;
%}

%union
{
	char *token;	
	/*Type type;
	
	Class *class;
	DeclList *decllist;
	VarDecl *vardecl;
	MethodDecl *methoddecl;
	ParamList *paramlist;
	VarDeclList *vardecllist;
	IDList *idlist;	
	StmtList *stmtlist;	
	Expr *expr;
	ArgsList *argslist;*/
}

%token <token> INT BOOL NEW IF ELSE WHILE PRINT PARSEINT CLASS PUBLIC STATIC VOID STRING DOTLENGTH RETURN AND OR RELCOMPAR BOOLLIT ID INTLIT RESERVED EQUALITY ADDITIVE MULTIPLIC

/*%type <class>		start
%type <decllist>	decls
%type <vardecl>		fielddecl
%type <methoddecl>	methoddecl
%type <paramlist>	formalparams formalparamslist
%type <vardecllist>	vardecl
%type <idlist>		vardecllist
%type <stmtlist>	stmtlist statement
%type <expr>		expr expr1 expr2
%type <argslist> 	args argslist
%type <type>		methodtype type*/

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
            | 																	 {}

formalparamslist: formalparamslist ',' type ID                                   {}
                | 																 {}

stmtlist: stmtlist statement                                                     {}
        | 																		 {}

vardecl: vardecl type ID vardecllist ';'                                          {}
	   | 																		 {}

vardecllist: vardecllist ',' ID                                                  {}
           | 																	 {}

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

int yyerror(char *s) {printf("Line %d, col %d: %s: %s\n", prevLineNo, prevColNo, s, yytext);}
