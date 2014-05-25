#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astNodes.h"
#include "symbols.h"

#define MAX_LLVM_TYPE_SIZE 15

extern Class* myProgram;
extern ClassTable* symbolsTable;
extern MethodTable* currentLocalTable;
Type curFunctionType;
int varNumber, ifNumber, whileNumber, indexNumber;

void genGlobalVar(VarDecl*);
void genMethod(MethodDecl*);
void genLocalVar(VarDecl*);
void genStmtList(StmtList*);
void genStmt(Stmt*);
int genExpr(Expr* expr);

void getTypeLLVM(char*, Type);
void getRetTypeLLVM(char*, Type);

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
    const char llvmOutArr[] = "@.%s = global i32 0\n";
    const char llvmOutInitLenght[] = "\tstore i32 0, i32* @.%s\n";

    getTypeLLVM(llvmType, varDecl->type);

    if(varDecl->type == INT_T || varDecl->type == BOOL_T)
        sprintf(initVal, "0");
    else
        sprintf(initVal, "null");

    IDList *aux = varDecl->idList;
    for(; aux != NULL; aux = aux->next)
    {
        if(varDecl->type != INT_T && varDecl->type != BOOL_T)
            printf(llvmOutArr, aux->id);
        printf(llvmOut, aux->id, llvmType, initVal);
    }

    printf("\n");
}

void genMethod(MethodDecl* methodDecl)
{
    curFunctionType = methodDecl->type;
    varNumber = ifNumber = whileNumber = indexNumber = 0;
    currentLocalTable = getLocalTable(methodDecl->id);

    char llvmType[MAX_LLVM_TYPE_SIZE];
    getRetTypeLLVM(llvmType, methodDecl->type);

    printf("define %s @%s(", llvmType, methodDecl->id);

    ParamList* aux = methodDecl->paramList;
    if(aux != NULL)
    {
        getTypeLLVM(llvmType, aux->type);
        if(aux->type == INTARRAY || aux->type == BOOLARRAY || aux->type == STRINGARRAY)
            printf("i32 %%.%s, ", aux->id);
        printf("%s %%%s", llvmType, aux->id);
        aux = aux->next;
    }
    for(; aux != NULL; aux = aux->next)
    {
        getTypeLLVM(llvmType, aux->type);
        if(aux->type == INTARRAY || aux->type == BOOLARRAY || aux->type == STRINGARRAY)
            printf(", i32 %%.%s", aux->id);
        printf(", %s %%%s", llvmType, aux->id);
    }

    printf(")\n{\n");

    VarDeclList* aux2 = methodDecl->varDeclList;
    for(; aux2 != NULL; aux2 = aux2->next)
        genLocalVar(aux2->varDecl);

    genStmtList(methodDecl->stmtList);

    printf("}\n\n");
}

void genLocalVar(VarDecl* varDecl)
{
    char llvmType[MAX_LLVM_TYPE_SIZE];
    const char llvmOut[] = "\t%%%s = alloca %s\n";
    const char llvmOutArr[] = "\t%%.%s = alloca i32\n";
    const char llvmOutInitLenght[] = "\tstore i32 0, i32* %%.%s\n";

    getTypeLLVM(llvmType, varDecl->type);

    IDList *aux = varDecl->idList;
    for(; aux != NULL; aux = aux->next)
    {
        if(varDecl->type != INT_T && varDecl->type != BOOL_T)
        {
            printf(llvmOutArr, aux->id);
            printf(llvmOutInitLenght, aux->id);
        }
        printf(llvmOut, aux->id, llvmType);
    }

    printf("\n");
}

void genStmtList(StmtList* list)
{
    StmtList* aux3 = list;
    for(; aux3 != NULL; aux3 = aux3->next)
        genStmt(aux3->stmt);
}

void genStmt(Stmt* stmt)
{
    if(stmt->type == CSTAT)
        genStmtList(stmt->stmtList);
    else if(stmt->type == IFELSE)
    {
        int compVarNumber = genExpr(stmt->expr1);
        printf("\tbr i1 %%%d, label %%ifthen%d, label %%ifelse%d\n\n", compVarNumber, ifNumber, ifNumber);

        printf("ifthen%d:\n", ifNumber);
        genStmt(stmt->stmt1);
        printf("\tbr label %%ifend%d\n\n", ifNumber);

        printf("ifelse%d:\n", ifNumber);
        genStmt(stmt->stmt2);
        printf("\tbr label %%ifend%d\n\n", ifNumber);

        printf("ifend%d:\n", ifNumber);

        ifNumber++;
    }
    else if(stmt->type == RETURN_T)
    {
        if(stmt->expr1 != NULL)
        {
            char llvmType[MAX_LLVM_TYPE_SIZE];
            getTypeLLVM(llvmType, curFunctionType);
            int exprVarNumber = genExpr(stmt->expr1);

            if(curFunctionType == INT_T || curFunctionType == BOOL_T)
                printf("\tret %s %%%d\n", llvmType, exprVarNumber);
            else
            {
                printf("\tret i32 %%.%d, %s %%%d\n", exprVarNumber, llvmType, exprVarNumber);
            }
        }
        else
            printf("\t ret void\n");
    }
    else if(stmt->type == WHILE_T)
    {
        printf("whilestart%d:\n", whileNumber);
        int exprVarNumber = genExpr(stmt->expr1);
        printf("\tbr i1 %%%d, label %%whiledo%d, label %%whileend%d\n\n", exprVarNumber, whileNumber, whileNumber);

        printf("whiledo%d:\n", whileNumber);
        genStmt(stmt->stmt1);

        printf("\nwhileend%d:\n", whileNumber);

        whileNumber++;
    }
    else if(stmt->type == PRINT_T);
    else if(stmt->type == STORE)
    {
        int isLocal = 1;
        char varDeclSymbol[5];
        char llvmType[MAX_LLVM_TYPE_SIZE];
        int exprVarNumber = genExpr(stmt->expr1);

        Type aux = -1;
        Type varType = getSymbolFromLocal(stmt->id);
        if(varType == aux)
        {
            isLocal = 0;
            varType = getSymbolFromGlobal(stmt->id);
        }

        sprintf(varDeclSymbol, isLocal ? "%%" : "@");

        getTypeLLVM(llvmType, varType);

        printf("\tstore %s %%%d, %s* %s%s\n", llvmType, exprVarNumber, llvmType, varDeclSymbol, stmt->id);
    }
    else if(stmt->type == STOREARRAY)
    {
        int isLocal = 1;
        char varDeclSymbol[5];
        char llvmType[MAX_LLVM_TYPE_SIZE];
        char llvmType2[MAX_LLVM_TYPE_SIZE];
        int indexVarNumber = genExpr(stmt->expr1);
        int exprVarNumber = genExpr(stmt->expr2);

        Type arrayType = getSymbolFromLocal(stmt->id);
        if(arrayType == -1)
        {
            isLocal = 0;
            arrayType = getSymbolFromGlobal(stmt->id);
        }

        sprintf(varDeclSymbol, isLocal ? "%%" : "@");

        getTypeLLVM(llvmType, arrayType);

        printf("\t%%%d = getelementptr %s %s%s, i32 %%%d\n", indexNumber++, llvmType, varDeclSymbol, stmt->id, indexVarNumber);

        strcpy(llvmType2, llvmType);
        llvmType2[strlen(llvmType2)-1] = '\0';
        printf("\tstore %s %%%d, %s %s%s\n", llvmType2, exprVarNumber, llvmType, varDeclSymbol, stmt->id);
    }
}

int genExpr(Expr* expr)
{

}

const char* llvmTypes[6] = {"void", "i32", "i1", "i32*", "i1*", "i8**"};
void getTypeLLVM(char* llvmType, Type type)
{
    sprintf(llvmType, "%s", llvmTypes[type]);
}

const char* llvmRetTypes[6] = {"void", "i32", "i1", "{i32, i32*}", "{i32, i1*}", "i8**"};
void getRetTypeLLVM(char* llvmType, Type type)
{
    sprintf(llvmType, "%s", llvmRetTypes[type]);
}
