#ifndef ASTNODES_H
#define ASTNODES_H

////
// Enums of nodes types.

typedef enum {VARDECL, METHODDECL} DeclType;
typedef enum {VOID_T, INT_T, BOOL_T, INTARRAY, BOOLARRAY, STRINGARRAY} Type;
typedef enum {IFELSE, RETURN_T, WHILE_T, PRINT_T, STORE, STOREARRAY} StmtType;
typedef enum {BINOP, UNOP, PARENTHESIS, IDorLIT, CALL, PARSEINT_T} ExprType;
typedef enum {PLUS, MINUS, MUL, DIV, REM, LESSER, GREATER, LEQ, GEQ, DIF, EQ, NOT, DOTLENGTH_T} OpType;

////
// Nodes definitions.

typedef struct _argsList ArgsList;

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

typedef struct _stmt
{
    StmtType type;
    Expr *expr1;
    struct _stmt *stmt1;
    union
    {
        Expr *expr2;
        struct _stmt *stmt2;
        //struct _stmtList *stmtList;
    };
} Stmt;

typedef struct _stmtList
{
    Stmt *stmt;
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
    struct _declList *next;
} DeclList;

typedef struct _class
{
	char *id;
	DeclList *declList;
} Class;

////
// Functions for node handling.

Class* insertClass(char*, DeclList*);
DeclList* insertDecl(DeclType, void*, DeclList*);
VarDecl* insertFieldDecl(Type, char*, IDList*);
VarDeclList* insertVarDecl(VarDeclList*, Type, char*, IDList*);
IDList* insertID(char*, IDList*);
StmtList* insertStmt(Stmt*, StmtList*);
ParamList* insertFormalParam(Type, char*, ParamList*, int);
MethodDecl* insertMethodDecl(Type, char*, ParamList*, VarDeclList*, StmtList*);

#endif
