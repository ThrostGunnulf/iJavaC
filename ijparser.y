%{
#include <stdio.h>
#include "astNodes.h"

extern int prevLineNo;
extern int prevColNo;
extern char *yytext;

Class* myProgram;
%}

%union
{
	char *token;	
	Type type;
	
	struct _class *class;
	DeclList *decllist;
	VarDecl *vardecl;
	MethodDecl *methoddecl;
	ParamList *paramlist;
	VarDeclList *vardecllist;
	IDList *idlist;	
	StmtList *stmtlist;
	Stmt *stmt;	
	Expr *expr;
	ArgsList *argslist;
}

%token <token> INT BOOL NEW IF ELSE WHILE PRINT PARSEINT CLASS PUBLIC STATIC VOID STRING DOTLENGTH RETURN AND OR RELCOMPAR BOOLLIT ID INTLIT RESERVED EQUALITY ADDITIVE MULTIPLIC

%type <class>		start
%type <decllist>	decls
%type <vardecl>		fielddecl
%type <methoddecl>	methoddecl
%type <paramlist>	formalparams formalparamslist
%type <vardecllist>	vardecl 
%type <idlist>		idlist
%type <stmtlist>	stmtlist
%type <stmt>		statement
%type <expr>		expr exprindex exprnotindex
%type <argslist> 	args argslist
%type <type>		methodtype type

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

start: CLASS ID '{' decls '}'             {$$=insertClass($2, $4); myProgram = $$;}
	 | CLASS ID '{' '}'                   {$$=insertClass($2, NULL); myProgram = $$;}

decls: decls fielddecl                    {$$=insertDecl(VARDECL, $2, $1);}
     | decls methoddecl                   {$$=insertDecl(METHODDECL, $2, $1);}
     | fielddecl                          {$$=insertDecl(VARDECL, $1, NULL);}
     | methoddecl                         {$$=insertDecl(METHODDECL, $1, NULL);};

fielddecl: STATIC type ID idlist ';' 	  {$$=insertFieldDecl($2, $3, $4);};

methoddecl: PUBLIC STATIC methodtype ID '(' formalparams ')' '{' vardecl stmtlist '}'    {};

methodtype: type                          {$$=$1;}
          | VOID                          {$$=VOID_T;};

formalparams: type ID formalparamslist			 {}
            | STRING '[' ']' ID                  {}
            | 									 {$$=NULL;};

formalparamslist: formalparamslist ',' type ID   {}
                | 						 		 {$$=NULL;}

stmtlist: stmtlist statement              {$$=insertStmt($2, $1);}
        | 								  {$$=NULL;};

vardecl: vardecl type ID idlist ';'  	  {}
	   | 								  {$$=NULL;};

idlist: idlist ',' ID          	  		  {$$=insertID($3, $1);}
      | 							 	  {$$=NULL;};

type: INT '[' ']'                         {$$=INTARRAY;}
    | BOOL '[' ']'                        {$$=BOOLARRAY;}
    | INT                                 {$$=INT_T;}
    | BOOL                                {$$=BOOL_T;};

statement: '{' stmtlist '}'                                                      {}
         | IF '(' expr ')' statement ELSE statement   %prec ELSE                 {}
         | IF '(' expr ')' statement                  %prec IFX                  {}
         | WHILE '(' expr ')' statement                                          {}
         | PRINT '(' expr ')' ';'                                                {}
         | ID '[' expr ']' '=' expr ';'                                          {}
         | ID '=' expr ';'                                                       {}
         | RETURN expr ';'                                                       {}
         | RETURN ';'                                                            {};

expr: exprindex				  %prec EXPR1REDUCE									  {}
	| exprindex '[' expr ']'  				                         			  {}
	| exprnotindex																  {};

exprindex: expr AND expr                                                              {}
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

exprnotindex: NEW INT '[' expr ']'                                                {}
     		| NEW BOOL '[' expr ']'                                               {};

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
