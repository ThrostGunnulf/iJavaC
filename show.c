#include <stdio.h>
#include "show.h"

#define TYPE_SIZE 15

void printDeclList(DeclList*);
void printFieldDecl(VarDecl*);
void printMethodDecl(MethodDecl*);
void printMethodParams(ParamList*);
void printVarDeclList(VarDeclList*);
void printStmtList(StmtList*, int);
void typeToString(Type, char*);

void printProgram(Class* myProgram)
{
    printf("Program\n");
    printf("\tId(%s)\n", myProgram->id);

    printDeclList(myProgram->declList);
}

void printDeclList(DeclList* list)
{
    DeclList* aux = list;
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->type == VARDECL)
            printFieldDecl(aux->varDecl);
        else if(aux->type == METHODDECL)
            printMethodDecl(aux->methodDecl);
    }
}

void printFieldDecl(VarDecl* decl)
{
    char toPrint[40], type[TYPE_SIZE];

    typeToString(decl->type, type);
    sprintf(toPrint, "\tVarDecl\n\t\tStatic %s\n", type);

    IDList* aux = decl->idList;
    for(; aux != NULL; aux = aux->next)
        printf("%s\t\tId(%s)\n", toPrint, aux->id);
}

void printMethodDecl(MethodDecl* decl)
{
    char type[TYPE_SIZE];
    typeToString(decl->type, type);

    printf("\tMethodDecl\n");
    printf("\t\t%s\n", type);
    printf("\t\tId(%s)\n", decl->id);
    printMethodParams(decl->paramList);
    printf("\t\tMethodBody\n");
    printVarDeclList(decl->varDeclList);
    printStmtList(decl->stmtList, 2);
}

void printMethodParams(ParamList* params)
{
    printf("\t\tMethodParams\n");

    char type[TYPE_SIZE];
    ParamList* aux = params;
    for(; aux != NULL; aux = aux->next)
    {
        printf("\t\t\tParamDeclaration\n");
        typeToString(aux->type, type);
        printf("\t\t\t\t%s\n", type);
        printf("\t\t\t\tId(%s)\n", aux->id);
    }
}

void printVarDeclList(VarDeclList* decls)
{
    char type[TYPE_SIZE];
    VarDeclList* aux = decls;

    for(; aux != NULL; aux = aux->next)
    {
        typeToString(aux->varDecl->type, type);
        IDList* aux2 = aux->varDecl->idList;
        for(; aux2 != NULL; aux2 = aux2->next)
        {
            printf("\t\t\tVarDecl\n");
            printf("\t\t\t\t%s\n", type);
            printf("\t\t\t\tId(%s)\n", aux2->id);
        }
    }
}

void printStmtList(StmtList* stmts, int depth)
{

}

void typeToString(Type type, char* dest)
{
    if(type == INT_T)
        sprintf(dest, "%s", "Int");
    else if(type == BOOL_T)
        sprintf(dest, "%s", "Bool");
    else if(type == INTARRAY)
        sprintf(dest, "%s", "IntArray");
    else if(type == BOOLARRAY)
        sprintf(dest, "%s", "BoolArray");
    else if(type == VOID_T)
        sprintf(dest, "%s", "Void");
    else if(type == STRINGARRAY)
        sprintf(dest, "%s", "StringArray");

}
