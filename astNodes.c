#include <stdlib.h>
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

IDList* insertID(char* id, IDList* list)
{
    IDList* newID = (IDList*) malloc(sizeof(IDList));
    newID->id = id;

    IDList *aux = list;
    for(; aux->next != NULL; aux = aux->next);
    aux->next = newID;

    return list;
}

StmtList* insertStmt(Stmt* stmt, StmtList* list)
{
    StmtList* newStmtList = (StmtList*) malloc(sizeof(StmtList));
    newStmtList->stmt = stmt;

    StmtList* aux = list;
    for(; aux->next != NULL; aux = aux->next);
    aux->next = newStmtList;

    return list;
}
