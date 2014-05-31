#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "astNodes.h"

////
// Global and local table structures.
typedef struct _methodTableEntry
{
    char* id;
    Type type;
    int isParam;
    struct _methodTableEntry* next;
} MethodTableEntry;

typedef struct _methodTable
{
    struct _classTable* broaderTable;
    MethodTableEntry* entries;

} MethodTable;

typedef struct _classTableEntry
{
    char* id;
    Type type;
    MethodTable* methodTable;
    struct _classTableEntry* next;
} ClassTableEntry;

typedef struct _classTable
{
    char* id;
    ClassTableEntry* entries;
} ClassTable;

////
// Functions for table creation.
ClassTable* buildSymbolsTables(Class*);

////
// Functions for symbol finding.
Type getMethodFromGlobal(char*);
Type getSymbol(char*);
Type getSymbolFromGlobal(char*);
Type getSymbolFromLocal(char*);
Type getSymbolFromLocalOrGlobal(char*);
int isLocalSymbolParam(char*);
MethodTable* getLocalTable(char*);

#endif
