%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "astNodes.h"
#include "show.h"
#include "symbols.h"
#include "semantic.h"
#include "exitClean.h"
#include "codeGen.h"

void checkFlags(int, char **, int*, int*);

extern int prevLineNo;
extern int prevColNo;
extern char *yytext;

int error = 0;
Class* myProgram = NULL;
ClassTable* symbolsTable = NULL;
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

methoddecl: PUBLIC STATIC methodtype ID '(' formalparams ')' '{' vardecl stmtlist '}' 
{$$=insertMethodDecl($3, $4, $6, $9, $10);};

methodtype: type                          {$$=$1;}
          | VOID                          {$$=VOID_T;};

formalparams: type ID formalparamslist			 {$$=insertFormalParam($1, $2, $3, 1);}
            | STRING '[' ']' ID                  {$$=insertFormalParam(STRINGARRAY, $4, NULL, 1);}
            | 									 {$$=NULL;};

formalparamslist: formalparamslist ',' type ID   {$$=insertFormalParam($3, $4, $1, 0);}
                | 						 		 {$$=NULL;}

stmtlist: stmtlist statement              {$$=insertStmtList($2, $1);}
        | 								  {$$=NULL;};

vardecl: vardecl type ID idlist ';'  	  {$$=insertVarDecl($1, $2, $3, $4);}
	   | 								  {$$=NULL;};

idlist: idlist ',' ID          	  		  {$$=insertID($3, $1);}
      | 							 	  {$$=NULL;};

type: INT '[' ']'                         {$$=INTARRAY;}
    | BOOL '[' ']'                        {$$=BOOLARRAY;}
    | INT                                 {$$=INT_T;}
    | BOOL                                {$$=BOOL_T;};

statement: '{' stmtlist '}'           {$$=insertStmt(CSTAT, NULL, NULL, NULL, NULL, NULL, $2);}
         | IF '(' expr ')' statement ELSE statement  %prec ELSE  {$$=insertStmt(IFELSE, NULL, $3, NULL, $5, $7, NULL);}
         | IF '(' expr ')' statement                 %prec IFX   {$$=insertStmt(IFELSE, NULL, $3, NULL, $5, NULL, NULL);}
         | WHILE '(' expr ')' statement  {$$=insertStmt(WHILE_T, NULL, $3, NULL, $5, NULL, NULL);}
         | PRINT '(' expr ')' ';'      {$$=insertStmt(PRINT_T, NULL, $3, NULL, NULL, NULL, NULL);}
         | ID '[' expr ']' '=' expr ';' {$$=insertStmt(STOREARRAY, $1, $3, $6, NULL, NULL, NULL);}
         | ID '=' expr ';'              {$$=insertStmt(STORE, $1, $3, NULL, NULL, NULL, NULL);}
         | RETURN expr ';'         {$$=insertStmt(RETURN_T, NULL, $2, NULL, NULL, NULL, NULL);}
         | RETURN ';'              {$$=insertStmt(RETURN_T, NULL, NULL, NULL, NULL, NULL, NULL);};

expr: exprindex			  			  {$$=$1;}
	| exprnotindex					  {$$=$1;};

exprindex: ID                              {$$=insertExpr(ID_T, NULL, NULL, NULL, $1, NULL);}
     	 | INTLIT                          {$$=insertExpr(INTLIT_T, NULL, NULL, NULL, $1, NULL);}
     	 | BOOLLIT                         {$$=insertExpr(BOOLLIT_T, NULL, NULL, NULL, $1, NULL);}
     	 | '(' expr ')'                     {$$=$2;}
     	 | expr DOTLENGTH                   {$$=insertExpr(UNOP, "DOT", $1, NULL, NULL, NULL);}
     	 | PARSEINT '(' ID '[' expr ']' ')' {$$=insertExpr(PARSEINT_T, $3, $5, NULL, $3, NULL);}
     	 | ID '(' args ')'                  {$$=insertExpr(CALL, NULL, NULL, NULL, $1, $3);}
     	 | ID '(' ')'                       {$$=insertExpr(CALL, NULL, NULL, NULL, $1, NULL);};
		 | exprindex '[' expr ']'  			{$$=insertExpr(INDEX, NULL, $1, $3, NULL, NULL);}

exprnotindex: NEW INT '[' expr ']'        {$$=insertExpr(NEWINTARR, NULL, $4, NULL, NULL, NULL);}
     		| NEW BOOL '[' expr ']'       {$$=insertExpr(NEWBOOLARR, NULL, $4, NULL, NULL, NULL);}
     	 	| '!' expr       %prec UNARY  {$$=insertExpr(UNOP, "!", $2, NULL, NULL, NULL);}
     	 	| ADDITIVE expr  %prec UNARY  {$$=insertExpr(UNOP, $1, $2, NULL, NULL, NULL);}
     	 	| expr AND expr               {$$=insertExpr(BINOP, $2, $1, $3, NULL, NULL);}
     	 	| expr OR expr                {$$=insertExpr(BINOP, $2, $1, $3, NULL, NULL);}
     	 	| expr RELCOMPAR expr         {$$=insertExpr(BINOP, $2, $1, $3, NULL, NULL);}
     	 	| expr EQUALITY expr          {$$=insertExpr(BINOP, $2, $1, $3, NULL, NULL);}
     	 	| expr ADDITIVE expr          {$$=insertExpr(BINOP, $2, $1, $3, NULL, NULL);}
     	 	| expr MULTIPLIC expr         {$$=insertExpr(BINOP, $2, $1, $3, NULL, NULL);};

args: expr argslist                {$$=insertArg($1, $2);}
    | expr                         {$$=insertArg($1, NULL);};
 
argslist: ',' args                 {$$=$2;};

%%

int main(int argc, char *argv[])
{
	int printTree, printSymbols;

	yyparse();
	if(error)
		exitClean(0);
	
	checkFlags(argc, argv, &printTree, &printSymbols);
	
	if(printTree)
		printProgram(myProgram);
	
	if(!printTree || printSymbols)
	{
		symbolsTable = buildSymbolsTables(myProgram);
		checkSemantics(myProgram);
	}
	
	if(printSymbols)
		printSymbolTables(symbolsTable);
	
    generateCode();

	//freeProgram(myProgram, symbolsTable); //TO IMPLEMENT
    return 0;
}

void checkFlags(int argc, char *argv[], int* printTree, int* printSymbols)
{	
	int i;
	*printTree = *printSymbols = 0;
	for(i=0; i < argc; i++)
	{
		if(strcmp(argv[i], "-s") == 0)
		{
			*printSymbols = 1;
		}
		else if(strcmp(argv[i], "-t") == 0)
		{
			*printTree = 1;
		}
	}
}

void exitClean(int exitValue)
{	
	//freeProgram(myProgram, symbolsTable); //TO IMPLEMENT
	exit(0);
}

int yyerror(char *s)
{
	printf("Line %d, col %d: %s: %s\n", prevLineNo, prevColNo, s, yytext);
	error=1;
	return -1;
}
