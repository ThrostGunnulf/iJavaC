#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "astNodes.h"

Class* insertClass(char* id, DeclList* declList)
{
    Class* newClass = (Class*) malloc(sizeof(Class));
    newClass->id = id;
    newClass->declList = declList;

    return newClass;
}

DeclList* insertDecl(DeclType type, void* decl, DeclList* list)
{
    DeclList* newDecl = (DeclList*) malloc(sizeof(DeclList));
    newDecl->type = type;
    if(type == VARDECL)
        newDecl->varDecl = (VarDecl*) decl;
    else
        newDecl->methodDecl = (MethodDecl*) decl;
    newDecl->next = NULL;

    if(list == NULL)
        return newDecl;

    DeclList* aux = list;
    for(; aux->next != NULL; aux = aux->next);
    aux->next = newDecl;

    return list;
}

VarDecl* insertFieldDecl(Type type, char* id, IDList* list)
{
    IDList* newId = (IDList*) malloc(sizeof(IDList));
    newId->id = id;
    newId->next = list;

    VarDecl* newVarDecl = (VarDecl*) malloc(sizeof(VarDecl));
    newVarDecl->type = type;
    newVarDecl->isStatic = 1;
    newVarDecl->idList = newId;

    return newVarDecl;
}

VarDeclList* insertVarDecl(VarDeclList* vardecl, Type type, char* id, IDList* list) 
{
	IDList* newId = (IDList*) malloc(sizeof(IDList));
    newId->id = id;
    newId->next = list;
	
    VarDecl* newVarDecl = (VarDecl*) malloc(sizeof(VarDecl));
	newVarDecl->type = type;
	newVarDecl->isStatic = 0;
	newVarDecl->idList = newId;

    VarDeclList* newVarDeclList = (VarDeclList*) malloc(sizeof(VarDeclList));
    newVarDeclList->varDecl = newVarDecl;
    newVarDeclList->next = NULL;
	
    if(vardecl==NULL)
        return newVarDeclList;

	VarDeclList* aux = vardecl;
	for(; aux->next != NULL; aux = aux->next);
	aux->next = newVarDeclList;
	
	return vardecl;
}

IDList* insertID(char* id, IDList* list)
{
    IDList* newID = (IDList*) malloc(sizeof(IDList));
    newID->id = id;
    newID->next = NULL;

    if(list == NULL)
        return newID;

    IDList *aux = list;
    for(; aux->next != NULL; aux = aux->next);
    aux->next = newID;

    return list;
}

StmtList* insertStmtList(Stmt* stmt, StmtList* list)
{
    if(!stmt)
        return list;

    StmtList* newStmtList = (StmtList*) malloc(sizeof(StmtList));
    newStmtList->stmt = stmt;
    newStmtList->next = NULL;

    if(list == NULL)
        return newStmtList;

    StmtList* aux = list;
    for(; aux->next != NULL; aux = aux->next);
    aux->next = newStmtList;

    return list;
}

Stmt* insertStmt(StmtType type, char* id, Expr* expr1, Expr* expr2, Stmt* stmt1, Stmt* stmt2, StmtList* stmtList)
{
    if(type == CSTAT)
    {
        if(stmtList == NULL)
            return NULL;
        else if(stmtList->next == NULL)
            return stmtList->stmt;
    }

    Stmt* newStmt = (Stmt*) malloc(sizeof(Stmt));
    newStmt->type = type;
    newStmt->id = id;
    newStmt->expr1 = expr1;
    newStmt->expr2 = expr2;
    newStmt->stmt1 = stmt1;
    newStmt->stmt2 = stmt2;
    newStmt->stmtList = stmtList;

    return newStmt;
}

ParamList* insertFormalParam(Type type, char* id, ParamList* list, int isHead)
{
    ParamList* newParam = (ParamList*) malloc(sizeof(ParamList));
    newParam->type = type;
    newParam->id = id;
    newParam->next = NULL;

    if(isHead || !list)
    {
        newParam->next = list;
        return newParam;
    }

    ParamList* aux = list;
    for(; aux->next != NULL; aux = aux->next);
    aux->next = newParam;

    return list;
}

MethodDecl* insertMethodDecl(Type type, char* id, ParamList* params, VarDeclList* decls, StmtList* stmts)
{
    MethodDecl* newMethodDecl = (MethodDecl*) malloc(sizeof(MethodDecl));
    newMethodDecl->type = type;
    newMethodDecl->id = id;
    newMethodDecl->paramList = params;
    newMethodDecl->varDeclList = decls;

    if(stmts && stmts->stmt->type == CSTAT && stmts->next == NULL)
    {
        StmtList* old = stmts;
        stmts = stmts->stmt->stmtList;
        free(old);
    }

    newMethodDecl->stmtList = stmts;

    return newMethodDecl;
}

OpType getOpType(char* op)
{
    if(strcmp(op, "&&") == 0)
        return AND_T;
    else if(strcmp(op, "||") == 0)
        return OR_T;
    else if(strcmp(op, "<") == 0)
        return LESSER;
    else if(strcmp(op, ">") == 0)
        return GREATER;
    else if(strcmp(op, "<=") == 0)
        return LEQ;
    else if(strcmp(op, ">=") == 0)
        return GEQ;
    else if(strcmp(op, "!=") == 0)
        return DIF;
    else if(strcmp(op, "==") == 0)
        return EQ;
    else if(strcmp(op, "!") == 0)
        return NOT;
    else if(strcmp(op, "+") == 0)
        return PLUS;
    else if(strcmp(op, "-") == 0)
        return MINUS;
    else if(strcmp(op, "*") == 0)
        return MUL;
    else if(strcmp(op, "/") == 0)
        return DIV;
    else if(strcmp(op, "%") == 0)
        return REM;
    else if(strcmp(op, "DOT") == 0)
        return DOTLENGTH_T;

    return -1;
}

Expr* insertExpr(ExprType type, char* op, Expr* expr1, Expr* expr2, char* idOrLit, ArgsList* argsList)
{
    Expr* newExpr = (Expr*) malloc(sizeof(Expr));
    newExpr->type = type;
    if(op != NULL)
        newExpr->op = getOpType(op);
    else
        newExpr->op = -1;
    newExpr->expr1 = expr1;
    newExpr->expr2 = expr2;
    newExpr->idOrLit = idOrLit;
    newExpr->argsList = argsList;

    return newExpr;
}

ArgsList* insertArg(Expr* expr, ArgsList* list)
{
    ArgsList* newArgList = (ArgsList*) malloc(sizeof(ArgsList));
    newArgList->expr = expr;
    newArgList->next = list;

    return newArgList;
}
