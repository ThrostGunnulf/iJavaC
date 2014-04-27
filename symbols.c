#include <stdlib.h>
#include "symbols.h"

ClassTableEntry* newVarEntries(VarDecl*, ClassTableEntry*);
void newMethodEntry(MethodDecl*, ClassTableEntry*, ClassTable*);
void newMethodTable(MethodTable*, ParamList*, VarDeclList*);

ClassTable* buildSymbolsTables(Class* myProgram)
{
    ClassTable* table = (ClassTable*) malloc(sizeof(ClassTable));
    table->id = myProgram->id;
    table->entries = NULL;

    DeclList* aux = myProgram->declList;
    ClassTableEntry* last = NULL, *end = NULL;
    for(; aux != NULL; aux = aux->next)
    {
        ClassTableEntry* newEntry = (ClassTableEntry*) malloc(sizeof(ClassTableEntry));
        end = newEntry;
        if(aux->type == VARDECL)
            end = newVarEntries(aux->varDecl, newEntry);
        else if(aux->type == METHODDECL)
            newMethodEntry(aux->methodDecl, newEntry, table);

        if(table->entries == NULL)
            table->entries = newEntry;
        else
            last->next = newEntry;

        last = end;
    }

    return table;
}

ClassTableEntry* newVarEntries(VarDecl* decl, ClassTableEntry* tableEntry)
{
    tableEntry->id = decl->idList->id;
    tableEntry->type = decl->type;
    tableEntry->next = NULL;

    ClassTableEntry* last = NULL;
    IDList* aux = decl->idList->next;
    for(; aux != NULL; aux = aux->next)
    {
        ClassTableEntry* newEntry = (ClassTableEntry*) malloc(sizeof(ClassTableEntry));
        newEntry->id = aux->id;
        newEntry->type = decl->type;
        newEntry->next = NULL;

        if(tableEntry->next = NULL)
            tableEntry->next = newEntry;
        else
            last->next = newEntry;
    }

    return last;
}

void newMethodEntry(MethodDecl* decl, ClassTableEntry* tableEntry, ClassTable* broaderTable)
{
    tableEntry->id = decl->id;
    tableEntry->type = decl->type;
    tableEntry->next = NULL;

    MethodTable* methodTable = (MethodTable*) malloc(sizeof(MethodTable));
    methodTable->broaderTable = broaderTable;
    methodTable->entries = NULL;
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
        MethodTableEntry* newEntry = (MethodTableEntry*) malloc(sizeof(MethodTableEntry));
        newEntry->id = aux->varDecl->idList->id;
        newEntry->type = aux->varDecl->type;
        newEntry->isParam = 0;
        newEntry->next = NULL;

        MethodTableEntry* last2 = newEntry;
        IDList* aux2 = aux->varDecl->idList->next;
        for(; aux2 != NULL; aux2 = aux2->next)
        {
            MethodTableEntry* newEntry2 = (MethodTableEntry*) malloc(sizeof(MethodTableEntry));
            newEntry2->id = aux2->id;
            newEntry2->type = aux->varDecl->type;
            newEntry2->isParam = 0;
            newEntry2->next = NULL;

            last2->next = newEntry2;
        }

        end = last2;

        if(methodTable->entries == NULL)
            methodTable->entries = newEntry;
        else
            last->next = newEntry;

        last = end;
    }
}
