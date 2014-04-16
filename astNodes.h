#ifndef ASTNODES_H
#define ASTNODES_H

typedef struct _argsList ArgsList;

typedef struct _expr
{
	int type;
	int op;
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
	int type;
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
	int type;
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
	int type;
	char *id;
	struct _paramList *next;
} ParamList;

typedef struct _methodDecl
{
	int type;
	char *id;
	ParamList *paramList;
	VarDeclList *varDeclList;
	StmtList *stmtList;
} MethodDecl;

typedef struct _declList
{
	int type;
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