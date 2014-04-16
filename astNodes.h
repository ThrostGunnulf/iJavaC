#ifndef ASTNODES_H
#define ASTNODES_H

typedef struct _argsList ArgsList;

typedef enum {VARDECL, METHODDECL} DeclType;
typedef enum {VOID_T, INT_T, BOOL_T, INTARRAY, BOOLARRAY, STRINGARRAY} Type;
typedef enum {IFELSE, RETURN_T, WHILE_T, PRINT_T, STORE, STOREARRAY} StmtType;
typedef enum {BINOP, UNOP, PARENTESIS, IDorLIT, CALL, PARSEINT_T} ExprType;
typedef enum {PLUS, MINUS, MUL, DIV, REM, LESSER, GREATER, LEQ, GEQ, DIF, EQ, NOT, DOTLENGTH_T} OpType;

typedef struct _expr
{
    ExprType type;
    OpType op;
	struct _expr *expr1;
	struct _expr *expr2;
	char *idOrLit;
	ArgsList *argsList;
} Expr;

struct _argsList
{
	Expr *expr;
	struct _argsList *argsList;
};

typedef struct _stmtList
{
    StmtType type;
	Expr *expr1;
	Expr *expr2;
	struct _stmtList *stmtList1;
	struct _stmtList *stmtList2;
	struct _stmtList *next;
} StmtList;

typedef struct _idList
{
	char *id;
	struct _idList *next;
} IDList;

typedef struct _varDecl
{
    Type type;
	int isStatic;
	IDList *idList;
} VarDecl;

typedef struct _varDeclList
{
	VarDecl *varDecl;
	struct _varDeclList *next; 
} VarDeclList;

typedef struct _paramList
{
    Type type;
	char *id;
	struct _paramList *next;
} ParamList;

typedef struct _methodDecl
{
    Type type;
	char *id;
	ParamList *paramList;
	VarDeclList *varDeclList;
	StmtList *stmtList;
} MethodDecl;

typedef struct _declList
{
    DeclType type;
	union
	{
		VarDecl *varDecl;
		MethodDecl *methodDecl;
	};
} DeclList;

typedef struct _class
{
	char *id;
	DeclList *declList;
} Class;

#endif
