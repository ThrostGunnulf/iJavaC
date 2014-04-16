%{
#define INITCOL 1

#define YYSTYPE_IS_DECLARED
#define YYSTYPE char*

#include <string.h>
#include "y.tab.h"

extern YYSTYPE yylval;

int colNo = INITCOL;
int lineScom = 0, colScom = 0;
%}

%option yylineno

reserved        (abstract|assert|break|byte|case|catch|char|const|continue|default|do|double|enum|extends|final|finally|float|for|goto|implements|import|instanceof|interface|long|native|package|private|protected|short|strictfp|super|switch|synchronized|this|throw|throws|transient|try|volatile|null|"++"|"--")

letter          [a-zA-Z_$]
alphanumeric    [a-zA-Z_$0-9]
hex             [0-9a-fA-F]
dec             [0-9]

%X COMMENT

%%
<COMMENT><<EOF>>                        {BEGIN 0; printf("Line %d, col %d: unterminated comment\n", lineScom, colScom);}
<COMMENT>"*/"                           {BEGIN 0; colNo += 2;}
<COMMENT>.                              {colNo++;}
<COMMENT>"\n"                           {colNo = INITCOL;}
"/*"                                    {BEGIN COMMENT; lineScom = yylineno; colScom = colNo; colNo += 2;}
"//".*                                  {colNo = INITCOL;}

" "|"\t"                                {colNo++;}
"\n"                                    {colNo = INITCOL;}
{reserved}                              {return RESERVED; yylval = strdup(yytext); colNo += yyleng;}

"int"                                   {colNo += yyleng; return INT;}
"boolean"                               {colNo += yyleng; return BOOL;}
"new"                                   {colNo += yyleng; return NEW;}
"if"                                    {colNo += yyleng; return IF;}
"else"                                  {colNo += yyleng; return ELSE;}
"while"                                 {colNo += yyleng; return WHILE;}
"System.out.println"                    {colNo += yyleng; return PRINT;}
"Integer.parseInt"                      {colNo += yyleng; return PARSEINT;}
"class"                                 {colNo += yyleng; return CLASS;}
"public"                                {colNo += yyleng; return PUBLIC;}
"static"                                {colNo += yyleng; return STATIC;}
"void"                                  {colNo += yyleng; return VOID;}
"String"                                {colNo += yyleng; return STRING;}
".length"                               {colNo += yyleng; return DOTLENGTH;}
"return"                                {colNo += yyleng; return RETURN;}
"("|")"|"{"|"}"|"["|"]"|"!"|"="|";"|"," {colNo += yyleng; return yytext[0];}
"&&"                                    {colNo += yyleng; return AND;}
"||"                                    {colNo += yyleng; return OR;}
"<"|">"|"<="|">="                       {colNo += yyleng; yylval = strdup(yytext); return RELCOMPAR;}
"!="|"=="                               {colNo += yyleng; yylval = strdup(yytext); return EQUALITY;}
"+"|"-"              					{colNo += yyleng; yylval = strdup(yytext); return ADDITIVE;}
"*"|"/"|"%"								{colNo += yyleng; yylval = strdup(yytext); return MULTIPLIC;}
"true"|"false"                          {colNo += yyleng; yylval = strdup(yytext); return BOOLLIT;}
{letter}{alphanumeric}*                 {colNo += yyleng; yylval = strdup(yytext); return ID;}
{dec}+|"0x"{hex}+                       {colNo += yyleng; yylval = strdup(yytext); return INTLIT;}

.                                       {printf("Line %d, col %d: illegal character ('%s')\n", yylineno, colNo, yytext); colNo++;}

%%

int yywrap()
{
    return 1;
}
