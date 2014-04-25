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

VarDeclList* insertVarDecl(VarDeclList* vardecl, Type type, char* id, IDList* list) 
{
	IDList* newId = (IDList*) malloc(sizeof(IDList));
    newId->id = id;
    newId->next = list;
	
	VarDecl* newVarDecl = (VarDecl*) malloc(sizeof(VarDecl*));
	newVarDecl->type = type;
	newVarDecl->isStatic = 0;
	newVarDecl->idList = newId;
	
	if(vardecl==NULL) {
		return newVarDecl;
	}
	
	VarDeclList* newVarDeclList = (VarDeclList*) malloc(sizeof(VarDeclList*));
	VarDeclList* aux = vardecl;
	for(; aux->next != NULL; aux = aux->next);
	aux->next = newVarDeclList;
	
	return vardecl;
}
