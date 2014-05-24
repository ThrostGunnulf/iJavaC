#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astNodes.h"
#include "symbols.h"

#define MAX_LLVM_TYPE_SIZE 5

extern Class* myProgram;
extern ClassTable* symbolsTable;

void genGlobalVar(VarDecl*);
void genMethod(MethodDecl*);
void genLocalVar(VarDecl*);
void genStmts(Stmt*, int);

void getTypeLLVM(char*, Type);

void generateCode()
{
    DeclList* aux = myProgram->declList;
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->type == VARDECL)
            genGlobalVar(aux->varDecl);
        else if(aux->type == METHODDECL)
            genMethod(aux->methodDecl);
    }
}

void genGlobalVar(VarDecl* varDecl)
{
    char initVal[5];
    char llvmType[MAX_LLVM_TYPE_SIZE];
    const char llvmOut[] = "@%s = global %s %s\n";

    getTypeLLVM(llvmType, varDecl->type);

    if(varDecl->type == INT_T || varDecl->type == BOOL_T)
        sprintf(initVal, "0");
    else
        sprintf(initVal, "null");

    IDList *aux = varDecl->idList;
    for(; aux != NULL; aux = aux->next)
    {
        printf(llvmOut, aux->id, llvmType, initVal);
    }

    printf("\n");
}

void genMethod(MethodDecl* methodDecl)
{
    char llvmType[MAX_LLVM_TYPE_SIZE];
    getTypeLLVM(llvmType, methodDecl->type);

    printf("define %s @%s(", llvmType, methodDecl->id);

    ParamList* aux = methodDecl->paramList;
    if(aux != NULL)
    {
        getTypeLLVM(llvmType, aux->type);
        printf("%s %%%s", llvmType, aux->id);
        aux = aux->next;
    }
    for(; aux != NULL; aux = aux->next)
    {
        getTypeLLVM(llvmType, aux->type);
        printf(", %s %%%s", llvmType, aux->id);
    }

    printf(")\n{\n");

    VarDeclList* aux2 = methodDecl->varDeclList;
    for(; aux2 != NULL; aux2 = aux2->next)
        genLocalVar(aux2->varDecl);

    StmtList* aux3 = methodDecl->stmtList;
    for(; aux3 != NULL; aux3 = aux3->next)
        genStmts(aux3->stmt, 1);

    printf("}\n\n");
}

void genLocalVar(VarDecl* varDecl)
{
    char llvmType[MAX_LLVM_TYPE_SIZE];
    const char llvmOut[] = "\t%%%s = alloca %s\n";

    getTypeLLVM(llvmType, varDecl->type);

    IDList *aux = varDecl->idList;
    for(; aux != NULL; aux = aux->next)
        printf(llvmOut, aux->id, llvmType);

    printf("\n");
}

void genStmts(Stmt* stmt, int depth)
{

}

char* llvmTypes[6] = {"void", "i32", "i1", "i32*", "i1*", "i8*"};
void getTypeLLVM(char* llvmType, Type type)
{
    sprintf(llvmType, "%s", llvmTypes[type]);
}
