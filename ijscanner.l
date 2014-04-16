%{
#define INITCOL  1
#define INITLINE 1

#define YYSTYPE_IS_DECLARED
#define YYSTYPE char*

#include <string.h>
#include "ijparser.tab.h"

void colCount(int l);

extern YYSTYPE yylval;

int colNo = INITCOL;
int prevColNo = INITCOL;
int lineNo = INITLINE;
int prevLineNo = INITLINE;
int lineScom = 0, colScom = 0;
%}

reserved        (abstract|assert|break|byte|case|catch|char|const|continue|default|do|double|enum|extends|final|finally|float|for|goto|implements|import|instanceof|interface|long|native|package|private|protected|short|strictfp|super|switch|synchronized|this|throw|throws|transient|try|volatile|null|"++"|"--")

letter          [a-zA-Z_$]
alphanumeric    [a-zA-Z_$0-9]
hex             [0-9a-fA-F]
dec             [0-9]

%X COMMENT

%%
<COMMENT><<EOF>>                        {BEGIN 0; printf("Line %d, col %d: unterminated comment\n", lineScom, colScom);}
<COMMENT>"*/"                           {BEGIN 0; colCount(yyleng);}
<COMMENT>.                              {colCount(yyleng);}
<COMMENT>"\n"                           {prevLineNo = lineNo++; prevColNo = colNo; colNo = INITCOL;}
"/*"                                    {BEGIN COMMENT; lineScom = yylineno; colScom = colNo; colCount(yyleng);}
"//".*                                  {prevColNo = colNo; colNo = INITCOL;}

" "|"\t"                                {colCount(yyleng);}
"\n"                                    {prevLineNo = lineNo++; prevColNo = colNo; colNo = INITCOL;}
{reserved}                              {return RESERVED; yylval = strdup(yytext); colCount(yyleng);}

"int"                                   {colCount(yyleng); return INT;}
"boolean"                               {colCount(yyleng); return BOOL;}
"new"                                   {colCount(yyleng); return NEW;}
"if"                                    {colCount(yyleng); return IF;}
"else"                                  {colCount(yyleng); return ELSE;}
"while"                                 {colCount(yyleng); return WHILE;}
"System.out.println"                    {colCount(yyleng); return PRINT;}
"Integer.parseInt"                      {colCount(yyleng); return PARSEINT;}
"class"                                 {colCount(yyleng); return CLASS;}
"public"                                {colCount(yyleng); return PUBLIC;}
"static"                                {colCount(yyleng); return STATIC;}
"void"                                  {colCount(yyleng); return VOID;}
"String"                                {colCount(yyleng); return STRING;}
".length"                               {colCount(yyleng); return DOTLENGTH;}
"return"                                {colCount(yyleng); return RETURN;}
"("|")"|"{"|"}"|"["|"]"|"!"|"="|";"|"," {colCount(yyleng); return yytext[0];}
"&&"                                    {colCount(yyleng); return AND;}
"||"                                    {colCount(yyleng); return OR;}
"<"|">"|"<="|">="                       {colCount(yyleng); yylval = strdup(yytext); return RELCOMPAR;}
"!="|"=="                               {colCount(yyleng); yylval = strdup(yytext); return EQUALITY;}
"+"|"-"              					{colCount(yyleng); yylval = strdup(yytext); return ADDITIVE;}
"*"|"/"|"%"								{colCount(yyleng); yylval = strdup(yytext); return MULTIPLIC;}
"true"|"false"                          {colCount(yyleng); yylval = strdup(yytext); return BOOLLIT;}
{letter}{alphanumeric}*                 {colCount(yyleng); yylval = strdup(yytext); return ID;}
{dec}+|"0x"{hex}+                       {colCount(yyleng); yylval = strdup(yytext); return INTLIT;}

.                                       {printf("Line %d, col %d: illegal character ('%s')\n", lineNo, colNo, yytext); colNo++;}

%%

void colCount(int l)
{
	prevLineNo = lineNo;
	prevColNo = colNo;
	colNo += l;
}

int yywrap()
{
    return 1;
}
