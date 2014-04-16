%{
#include <stdio.h>

void yyerror(char *s);
%}

%token INT BOOL NEW IF ELSE WHILE PRINT PARSEINT CLASS PUBLIC STATIC VOID STRING DOTLENGTH RETURN OCURV CCURV OBRACE CBRACE OSQUARE CSQUARE AND OR LESSEQUAL GREATEQUAL NOT ASSIGN SEMIC COMMA BOOLLIT ID INTLIT RESERVED NOTEQUAL EQUAL

%left OR
%left AND

%left EQUAL
%left NOTEQUAL

%left '<'
%left '>'
%left GREATEQUAL
%left LESSEQUAL

%left '+'
%left '-'

%left '*'
%left '/'
%left '%'

%right NOT
%right UMINUS
%right UPLUS

%right OSQUARE
%left DOTLENGTH

%nonassoc IFX
%nonassoc ELSE

%%

start: CLASS ID OBRACE decls CBRACE                                                        {};

decls: fielddecl decls                                                                     {}
     | methoddecl decls                                                                    {}
     | fielddecl                                                                           {}
     | methoddecl                                                                          {}
     | ;

fielddecl: STATIC vardecl                                                                  {};

methoddecl: PUBLIC STATIC methodtype ID OCURV formalparams CCURV OBRACE stmtlist CBRACE    {};

methodtype: type                                                                           {}
          | VOID                                                                           {};

formalparams: type ID formalparamslist
            | STRING OSQUARE CSQUARE ID                                                    {}
            | ;

formalparamslist: COMMA type ID formalparamslist                                           {}
                | ;

stmtlist: statement stmtlist                                                               {}
        | ;

vardecl: type ID vardecllist SEMIC                                                         {};

vardecllist: COMMA ID vardecllist                                                          {}
           | ;

type: INT OSQUARE CSQUARE                                                                  {}
    | BOOL OSQUARE CSQUARE                                                                 {}
    | INT                                                                                  {}
    | BOOL                                                                                 {};

statement: OBRACE statement CBRACE                                                         {}
         | IF OCURV expr CCURV statement ELSE statement   %prec ELSE                       {}
         | IF OCURV expr CCURV statement                  %prec IFX                        {}
         | WHILE OCURV expr CCURV statement                                                {}
         | PRINT OCURV expr CCURV SEMIC                                                    {}
         | ID OSQUARE expr CSQUARE ASSIGN expr SEMIC                                       {}
         | ID ASSIGN expr SEMIC                                                            {}
         | RETURN expr SEMIC                                                               {}
         | RETURN SEMIC                                                                    {};

expr: expr AND expr                                                                        {}
    | expr OR expr                                                                         {}
    | expr LESSEQUAL expr                                                                  {}
    | expr GREATEQUAL expr                                                                 {}
    | expr '<' expr                                                                        {}
    | expr '>' expr                                                                        {}
    | expr '+' expr                                                                        {}
    | expr '-' expr                                                                        {}
    | expr '*' expr                                                                        {}
    | expr '/' expr                                                                        {}
    | expr '%' expr                                                                        {}
    | expr OSQUARE expr CSQUARE                                                            {}
    | ID                                                                                   {}
    | INTLIT                                                                               {}
    | BOOLLIT                                                                              {}
    | NEW INT OSQUARE expr CSQUARE                                                         {}
    | NEW BOOL OSQUARE expr CSQUARE                                                        {}
    | OCURV expr CCURV                                                                     {}
    | expr DOTLENGTH                                                                       {}
    | NOT expr                                                                             {}
    | '+' expr     %prec UPLUS                                                             {}
    | '-' expr     %prec UMINUS                                                            {}
    | PARSEINT OCURV ID OSQUARE expr CSQUARE CCURV                                         {}
    | ID OCURV args CCURV                                                                  {}
    | ID OCURV CCURV                                                                       {};

args: expr argslist                                                                        {}
    | expr                                                                                 {};
 
argslist: COMMA args                                                                       {};

%%

int main()
{
    yyparse();    return 0;
}

void yyerror(char *s) {printf("%s\n", s);}
