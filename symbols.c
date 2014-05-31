#include <stdio.h>
#include <stdlib.h>
#include "symbols.h"
#include "exitClean.h"

////
// Functions for symbol finding.
extern ClassTable* symbolsTable;
extern MethodTable* currentLocalTable;

////
// Semantic errors functions.
void errorAlreadyDefined(char*);

////
// Functions for table creation.
ClassTableEntry* newVarEntries(VarDecl*, ClassTableEntry*);
void newMethodEntry(MethodDecl*, ClassTableEntry*, ClassTable*);
void newMethodTable(MethodTable*, ParamList*, VarDeclList*);

ClassTable* buildSymbolsTables(Class* myProgram)
{
    ClassTable* table = (ClassTable*) malloc(sizeof(ClassTable));
    table->id = myProgram->id;
    table->entries = NULL;
    symbolsTable = table;

    DeclList* aux = myProgram->declList;
    ClassTableEntry* last = NULL, *end = NULL;
    for(; aux != NULL; aux = aux->next)
    {
        ClassTableEntry* newEntry = (ClassTableEntry*) malloc(sizeof(ClassTableEntry));
        newEntry->id = NULL;
        newEntry->methodTable = NULL;
        newEntry->next = NULL;
        end = newEntry;

        if(table->entries == NULL)
            table->entries = newEntry;
        else
            last->next = newEntry;

        if(aux->type == VARDECL)
            end = newVarEntries(aux->varDecl, newEntry);
        else if(aux->type == METHODDECL)
            newMethodEntry(aux->methodDecl, newEntry, table);

        last = end;
    }

    return table;
}

ClassTableEntry* newVarEntries(VarDecl* decl, ClassTableEntry* tableEntry)
{
    char* id = decl->idList->id;
    if(getSymbol(id) != -1)
        errorAlreadyDefined(id);

    tableEntry->id = id;
    tableEntry->type = decl->type;
    tableEntry->next = NULL;

    ClassTableEntry* last = tableEntry;
    IDList* aux = decl->idList->next;
    for(; aux != NULL; aux = aux->next)
    {
        if(getSymbol(aux->id) != -1)
            errorAlreadyDefined(aux->id);

        ClassTableEntry* newEntry = (ClassTableEntry*) malloc(sizeof(ClassTableEntry));
        newEntry->id = aux->id;
        newEntry->type = decl->type;
        newEntry->methodTable = NULL;
        newEntry->next = NULL;

        if(tableEntry->next == NULL)
            tableEntry->next = newEntry;
        else
            last->next = newEntry;

        last = newEntry;
    }

    return last;
}

void newMethodEntry(MethodDecl* decl, ClassTableEntry* tableEntry, ClassTable* broaderTable)
{
    char* id = decl->id;

    if(getSymbol(id) != -1)
        errorAlreadyDefined(id);

    tableEntry->id = decl->id;
    tableEntry->type = decl->type;
    tableEntry->next = NULL;

    MethodTable* methodTable = (MethodTable*) malloc(sizeof(MethodTable));
    methodTable->broaderTable = broaderTable;
    methodTable->entries = NULL;
    currentLocalTable = methodTable;
    newMethodTable(methodTable, decl->paramList, decl->varDeclList);

    tableEntry->methodTable = methodTable;
}

void newMethodTable(MethodTable* methodTable, ParamList* params, VarDeclList* decls)
{
    ////
    // Parameters.
    ParamList* auxP = params;
    MethodTableEntry* last = NULL;
    for(; auxP != NULL; auxP = auxP->next)
    {
        if(getSymbolFromLocal(auxP->id) != -1)
            errorAlreadyDefined(auxP->id);

        MethodTableEntry* newEntry = (MethodTableEntry*) malloc(sizeof(MethodTableEntry));
        newEntry->id = auxP->id;
        newEntry->type = auxP->type;
        newEntry->isParam = 1;
        newEntry->next = NULL;

        if(methodTable->entries == NULL)
            methodTable->entries = newEntry;
        else
            last->next = newEntry;

        last = newEntry;
    }

    ////
    // Local variables.
    VarDeclList* aux = decls;
    MethodTableEntry* end = NULL;
    for(; aux != NULL; aux = aux->next)
    {
        char* id = aux->varDecl->idList->id;
        if(getSymbolFromLocal(id) != -1)
            errorAlreadyDefined(id);

        MethodTableEntry* newEntry = (MethodTableEntry*) malloc(sizeof(MethodTableEntry));
        newEntry->id = id;
        newEntry->type = aux->varDecl->type;
        newEntry->isParam = 0;
        newEntry->next = NULL;        

        if(methodTable->entries == NULL)
            methodTable->entries = newEntry;
        else
            last->next = newEntry;

        MethodTableEntry* last2 = newEntry;
        IDList* aux2 = aux->varDecl->idList->next;
        for(; aux2 != NULL; aux2 = aux2->next)
        {
            if(getSymbolFromLocal(aux2->id) != -1)
                errorAlreadyDefined(aux2->id);

            MethodTableEntry* newEntry2 = (MethodTableEntry*) malloc(sizeof(MethodTableEntry));
            newEntry2->id = aux2->id;
            newEntry2->type = aux->varDecl->type;
            newEntry2->isParam = 0;
            newEntry2->next = NULL;

            last2->next = newEntry2;
            last2 = newEntry2;
        }
        end = last2;

        last = end;
    }
}

////
// Functions for symbol finding.
Type getSymbol(char* id)
{
    ClassTableEntry* aux = symbolsTable->entries;
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->id && (strcmp(id, aux->id) == 0))
            return aux->type;
    }

    return -1;
}

Type getMethodFromGlobal(char* id)
{
    ClassTableEntry* aux = symbolsTable->entries;
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->id && aux->methodTable && (strcmp(id, aux->id) == 0))
            return aux->type;
    }

    return -1;
}

Type getSymbolFromGlobal(char* id)
{
    ClassTableEntry* aux = symbolsTable->entries;
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->id && !aux->methodTable && (strcmp(id, aux->id) == 0))
            return aux->type;
    }

    return -1;
}

Type getSymbolFromLocal(char* id)
{
    MethodTableEntry* aux = currentLocalTable->entries;
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->id && (strcmp(id, aux->id) == 0))
            return aux->type;
    }

    return -1;
}

Type getSymbolFromLocalOrGlobal(char* id)
{
    Type ret = -1;
    if((ret = getSymbolFromLocal(id)) == -1)
        ret = getSymbolFromGlobal(id);

    return ret;
}

MethodTable* getLocalTable(char* id)
{
    ClassTableEntry* aux = symbolsTable->entries;
    for(; aux != NULL; aux = aux->next)
    {
        if(strcmp(id, aux->id) == 0)
            return aux->methodTable;
    }

    return NULL;
}

int isLocalSymbolParam(char* id)
{
    MethodTableEntry* aux = currentLocalTable->entries;
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->id && (strcmp(id, aux->id) == 0) && aux->isParam)
            return 1;
    }

    return 0;
}

void errorAlreadyDefined(char* id)
{
    printf("Symbol %s already defined\n", id);
    exitClean(0);
}
