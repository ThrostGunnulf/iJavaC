#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astNodes.h"
#include "symbols.h"

#define MAX_LLVM_TYPE_SIZE 15

typedef struct _ExprRet
{
    int tempVarNum;
    Type type;
} ExprRet;

extern Class* myProgram;
extern ClassTable* symbolsTable;
extern MethodTable* currentLocalTable;
Type curFunctionType;
int varNumber, ifNumber, whileNumber, indexNumber, andNumber, orNumber;

void genPreamble();
void genGlobalVar(VarDecl*);
void genMethod(MethodDecl*);
void genLocalVar(VarDecl*);
void genStmtList(StmtList*);
void genStmt(Stmt*);
ExprRet buildExpression(Expr*,int,int,char*);
ExprRet genExpr(Expr*);

void getTypeLLVM(char*, Type);
void getRetTypeLLVM(char*, Type);

void generateCode()
{
    genPreamble();

    DeclList* aux = myProgram->declList;
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->type == VARDECL)
            genGlobalVar(aux->varDecl);
        else if(aux->type == METHODDECL)
            genMethod(aux->methodDecl);
    }
}

void genPreamble()
{
    printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*) nounwind readonly\n");
    printf("\n");
    printf("@str.d = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\"\n");
    printf("@str.f = private unnamed_addr constant [7 x i8] c\"false\\0A\\00\"\n");
    printf("@str.f = private unnamed_addr constant [7 x i8] c\"true\\0A\\00\\00\"\n");
    printf("@str.bools = global [2 x i8*] [i8* getelementptr inbounds ([7 x i8]* @str.f, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8]* @str.t, i32 0, i32 0)]\n");
    printf("\n");
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
    varNumber = ifNumber = whileNumber = indexNumber = andNumber = orNumber = 0;
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
        int compVarNumber = genExpr(stmt->expr1).tempVarNum;
        printf("\tbr i1 %%%d, label %%ifthen%d, label %%ifelse%d\n\n", compVarNumber, ifNumber, ifNumber);

        printf("ifthen%d:\n", ifNumber);
        if(stmt->stmt1 != NULL)
            genStmt(stmt->stmt1);
        printf("\tbr label %%ifend%d\n\n", ifNumber);

        printf("ifelse%d:\n", ifNumber);
        if(stmt->stmt2 != NULL)
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
            int exprVarNumber = genExpr(stmt->expr1).tempVarNum;

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
        int exprVarNumber = genExpr(stmt->expr1).tempVarNum;
        printf("\tbr i1 %%%d, label %%whiledo%d, label %%whileend%d\n\n", exprVarNumber, whileNumber, whileNumber);

        printf("whiledo%d:\n", whileNumber);
        if(stmt->stmt1 != NULL)
            genStmt(stmt->stmt1);

        printf("\nwhileend%d:\n", whileNumber);

        whileNumber++;
    }
    else if(stmt->type == PRINT_T)
    {
        ExprRet ret = genExpr(stmt->expr1);
        if(ret.type == INT_T)
        {
            printf("call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str2, i32 0, i32 0), i32 %%%d)\n", ret.tempVarNum);
        }
        else if(ret.type == BOOL_T)
        {
            printf("%%%d = zext i1 %%%d to i32\n", varNumber++, ret.tempVarNum);
            printf("%%%d = getelementptr inbounds [2 x i8*]* @%s, i32 0, i32 %%%d\n", varNumber++, stmt->id, varNumber);
            printf("%%%d = load i8** %%d\n", varNumber++, varNumber);
            printf("call i32 (i8*, ...)* @printf(i8* %%%d)\n", varNumber);

        }
    }
    else if(stmt->type == STORE)
    {
        int isLocal = 1;
        char varDeclSymbol[5];
        char llvmType[MAX_LLVM_TYPE_SIZE];
        int exprVarNumber = genExpr(stmt->expr1).tempVarNum;

        Type aux = -1;
        Type varType = getSymbolFromLocal(stmt->id);
        if(varType == aux)
        {
            isLocal = 0;
            varType = getSymbolFromGlobal(stmt->id);
        }

        sprintf(varDeclSymbol, isLocal ? "%%" : "@");

        getTypeLLVM(llvmType, varType);

        if(stmt->expr1->type == INTLIT_T || stmt->expr1->type == BOOLLIT_T)
        {
            printf("\tstore %s %d, %s* %s%s\n", llvmType, exprVarNumber, llvmType, varDeclSymbol, stmt->id);
        }
        else
        {
            printf("\tstore %s %%%d, %s* %s%s\n", llvmType, exprVarNumber, llvmType, varDeclSymbol, stmt->id);
        }
    }
    else if(stmt->type == STOREARRAY)
    {
        int isLocal = 1;
        char varDeclSymbol[5];
        char llvmType[MAX_LLVM_TYPE_SIZE];
        char llvmType2[MAX_LLVM_TYPE_SIZE];
        int indexVarNumber = genExpr(stmt->expr1).tempVarNum;
        int exprVarNumber = genExpr(stmt->expr2).tempVarNum;

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

ExprRet buildExpression(Expr* expr, int leftExpr, int rightExpr, char* operation)
{
    int returnValue = varNumber++;

    if(expr->expr1->type == INTLIT_T && expr->expr2->type == INTLIT_T)
    {
        printf("\t%%%d = %s i32 %d, %d\n", returnValue, operation, leftExpr, rightExpr);
    }
    else if(expr->expr1->type == INTLIT_T)
    {
        printf("\t%%%d = %s i32 %d, %%%d\n", returnValue, operation, leftExpr, rightExpr);
    }
    else if(expr->expr2->type == INTLIT_T)
    {
        printf("\t%%%d = %s i32 %%%d, %d\n", returnValue, operation, leftExpr, rightExpr);
    }
    else
    {
        printf("\t%%%d = %s i32 %%%d, %%%d\n", returnValue, operation, leftExpr, rightExpr);
    }

    return returnValue;
}

ExprRet genExpr(Expr* expr)
{
    int returnValue;
    int leftExprId, rightExprId;

    if(expr->type == BINOP)
    {
        leftExprId = genExpr(expr->expr1);
        rightExprId = genExpr(expr->expr2);

        if(expr->op == PLUS)
        {
            returnValue = buildExpression(expr, leftExprId, rightExprId, "add");
        }
        else if(expr->op == MINUS)
        {
            returnValue = buildExpression(expr, leftExprId, rightExprId, "sub");
        }
        else if(expr->op == MUL)
        {
            returnValue = buildExpression(expr, leftExprId, rightExprId, "mul");
        }
        else if(expr->op == DIV)
        {
            returnValue = buildExpression(expr, leftExprId, rightExprId, "sdiv");
        }
        else if(expr->op == REM)
        {
            returnValue = buildExpression(expr, leftExprId, rightExprId, "srem");
        }
        else if(expr->op == LESSER)
        {
            returnValue = buildExpression(expr, leftExprId, rightExprId, "icmp slt");
        }
        else if(expr->op == GREATER)
        {
            returnValue = buildExpression(expr, leftExprId, rightExprId, "icmp sgt");
        }
        else if(expr->op == LEQ)
        {
            returnValue = buildExpression(expr, leftExprId, rightExprId, "icmp sle");
        }
        else if(expr->op == GEQ)
        {
            returnValue = buildExpression(expr, leftExprId, rightExprId, "icmp sge");
        }
        else if(expr->op == DIF)
        {
            returnValue = buildExpression(expr, leftExprId, rightExprId, "icmp ne");
        }
        else if(expr->op == EQ)
        {
            returnValue = buildExpression(expr, leftExprId, rightExprId, "icmp eq");
        }
        else if(expr->op == AND_T)
        {
            int tempId;

            leftExprId = genExpr(expr->expr1);

            tempId = varNumber++;
            printf("%%%d = icmp ne i32 %%%d, 0", tempId, leftExprId);

            printf("\tbr i1 %%%d, label %%andTrue%d, label %%andFalse%d\n\n", tempId, andNumber, andNumber);

            printf("andTrue%d:\n", andNumber);
            rightExprId = genExpr(expr->expr2);
            printf("\t%%%d = icmp ne i32 %%%d, 0\n", varNumber, rightExprId);
            tempId = varNumber++;
            printf("\tbr label %%andFalse%d\n\n", andNumber);

            printf("andFalse%d\n", andNumber);


        }
        else if(expr->op == OR_T)
        {

        }

        return returnValue;
    }
    else if(expr->type == UNOP)
    {
        int exprId;

        if(expr->op == PLUS) //Not necessary
        {
            exprId = genExpr(expr->expr1);
            returnValue = varNumber++;
            printf("%%%d = add i32 0, %%%d", returnValue, exprId);
        }
        else if(expr->op == MINUS)
        {
            exprId = genExpr(expr->expr1);
            returnValue = varNumber++;
            printf("%%%d = sub i32 0, %%%d", returnValue, exprId);
        }
        else if(expr->op == NOT)
        {
            int tempId;
            exprId = genExpr(expr->expr1);
            returnValue = varNumber++;
            printf("\t%%%d = icmp ne i32 %%%d, 0\n", returnValue, exprId);
            printf("\t%%%d = xor i1 %%%d, true\n", varNumber, returnValue);
            returnValue = varNumber++;
            printf("\t%%%d = zext i1 %%%d to i32\n", varNumber, returnValue);
            returnValue = varNumber++;
        }
    }
    else if(expr->type == ID_T)
    {
        returnValue = varNumber++;
        printf("\t%%%d = load i32* %%%s\n", returnValue, expr->idOrLit);
    }
    else if(expr->type == INTLIT_T)
    {
        returnValue = (int) strtol(expr->idOrLit, NULL, 0);
    }
    else if(expr->type == BOOLLIT_T)
    {
        if(strcmp(expr->idOrLit, "true")) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else if(expr->type == CALL)
    {

    }
    else if(expr->type == PARSEINT_T) {

    }
    else if(expr->type == INDEX) {

    }
    else if(expr->type == NEWINTARR) {

    }
    else if(expr->type == NEWBOOLARR) {

    }

    return returnValue;
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
