#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astNodes.h"
#include "symbols.h"

#define MAX_LLVM_TYPE_SIZE 15
#define MAX_LLVM_OP_STRING 10

typedef struct _ExprRet
{
    int tempVarNum;
    Type type;
} ExprRet;

extern Class* myProgram;
extern ClassTable* symbolsTable;
extern MethodTable* currentLocalTable;

char prevLabel[100];
char* argCountName;
Type curFunctionType;
char* curFunctionName;
int varNumber, ifNumber, whileNumber, indexNumber, andNumber, orNumber;

void genPreamble();
void genGlobalVar(VarDecl*);
void genMethod(MethodDecl*);
void genLocalVar(VarDecl*);
void genStmtList(StmtList*);
void genStmt(Stmt*);
ExprRet buildExpression(Expr*,ExprRet,ExprRet,char*);
ExprRet genExpr(Expr*);

void getTypeLLVM(char*, Type);
void getOpLLVM(char*, OpType);

void generateCode()
{
    ifNumber = whileNumber = andNumber = orNumber = 0;
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
    printf("declare noalias i8* @calloc(i32, i32) nounwind\n");
    printf("\n");
    printf("@str.d = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\"\n");
    printf("@str.f = private unnamed_addr constant [7 x i8] c\"false\\0A\\00\"\n");
    printf("@str.t = private unnamed_addr constant [7 x i8] c\"true\\0A\\00\\00\"\n");
    printf("@str.bools = global [2 x i8*] [i8* getelementptr inbounds ([7 x i8]* @str.f, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8]* @str.t, i32 0, i32 0)]\n");
    printf("\n");
    printf("%%Int.Array = type { i32, i32* }\n");
    printf("%%Bool.Array = type { i32, i1* }\n");
    printf("\n");
}

void genGlobalVar(VarDecl* varDecl)
{
    char initVal[20];
    char llvmType[MAX_LLVM_TYPE_SIZE];
    const char llvmOut[] = "@%s = global %s %s\n";

    getTypeLLVM(llvmType, varDecl->type);

    if(varDecl->type == INT_T || varDecl->type == BOOL_T)
        sprintf(initVal, "0");
    else if(varDecl->type == INTARRAY)
        sprintf(initVal, "{i32 0, i32* null}");
    else if(varDecl->type == BOOLARRAY)
        sprintf(initVal, "{i32 0, i1* null}");
    else
        sprintf(initVal, "null");

    IDList *aux = varDecl->idList;
    for(; aux != NULL; aux = aux->next)
        printf(llvmOut, aux->id, llvmType, initVal);

    printf("\n");
}

void genMethod(MethodDecl* methodDecl)
{
    varNumber = indexNumber = 1;
    curFunctionType = methodDecl->type;
    curFunctionName = methodDecl->id;
    currentLocalTable = getLocalTable(methodDecl->id);

    //If generating main, adapt the type
    char llvmType[MAX_LLVM_TYPE_SIZE];
    if(strcmp(methodDecl->id, "main") == 0)
        getTypeLLVM(llvmType, INT_T);
    else
        getTypeLLVM(llvmType, methodDecl->type);

    printf("define %s @%s(", llvmType, methodDecl->id);

    ParamList* aux = methodDecl->paramList;
    if(aux != NULL)
    {
        getTypeLLVM(llvmType, aux->type);
        if(aux->type == STRINGARRAY) //If generating main, adapt the parameters
        {
            printf("i32 %%%s.len, ", aux->id);
            argCountName = aux->id;
        }
        printf("%s %%%s.param", llvmType, aux->id);

        aux = aux->next;
    }
    for(; aux != NULL; aux = aux->next)
    {
        getTypeLLVM(llvmType, aux->type);
        if(aux->type == STRINGARRAY) //If generating main, adapt the parameters
        {
            printf("i32 %%%s.len", aux->id);
            argCountName = aux->id;
        }
        printf(", %s %%%s.param", llvmType, aux->id);
    }

    printf(")\n{\n");

    //Save arguments to stack
    MethodTable* localTable = getLocalTable(methodDecl->id);
    MethodTableEntry* aux3 = localTable->entries;
    for(; aux3 != NULL; aux3 = aux3->next)
        if(aux3->isParam)
        {
            getTypeLLVM(llvmType, aux3->type);
            printf("\t%%%s = alloca %s\n", aux3->id, llvmType);
            printf("\tstore %s %%%s.param, %s* %%%s\n", llvmType, aux3->id, llvmType, aux3->id);
        }

    //Generate variable definition code
    VarDeclList* aux2 = methodDecl->varDeclList;
    for(; aux2 != NULL; aux2 = aux2->next)
        genLocalVar(aux2->varDecl);

    //Generate statements code
    genStmtList(methodDecl->stmtList);

    //Add a default return
    if(strcmp(methodDecl->id, "main") == 0)
        printf("\tret i32 0\n");
    else
    {
        if(methodDecl->type == VOID_T)
            printf("\tret void\n");
        else if(methodDecl->type == INT_T)
            printf("\tret i32 0\n");
        else if(methodDecl->type == BOOL_T)
            printf("\tret i1 0\n");
        else if(methodDecl->type == INTARRAY)
            printf("\tret %%Int.Array {i32 0, i32* null}\n");
        else if(methodDecl->type == BOOLARRAY)
            printf("\tret %%Bool.Array {i32 0, i1* null}\n");
    }

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
        int thisIfElseNumber = ifNumber++;

        int compVarNumber = genExpr(stmt->expr1).tempVarNum;
        printf("\tbr i1 %%%d, label %%if.then%d, label %%if.else%d\n\n", compVarNumber, thisIfElseNumber, thisIfElseNumber);

        printf("if.then%d:\n", thisIfElseNumber);
        if(stmt->stmt1 != NULL)
            genStmt(stmt->stmt1);
        printf("\tbr label %%if.end%d\n\n", thisIfElseNumber);

        printf("if.else%d:\n", thisIfElseNumber);
        if(stmt->stmt2 != NULL)
            genStmt(stmt->stmt2);
        printf("\tbr label %%if.end%d\n\n", thisIfElseNumber);

        printf("if.end%d:\n", thisIfElseNumber);
    }
    else if(stmt->type == RETURN_T)
    {
        if(stmt->expr1 != NULL)
        {
            char llvmType[MAX_LLVM_TYPE_SIZE];
            getTypeLLVM(llvmType, curFunctionType);
            int exprVarNumber = genExpr(stmt->expr1).tempVarNum;

            printf("\tret %s %%%d\n", llvmType, exprVarNumber);
        }
        else
        {
            if(strcmp(curFunctionName, "main") != 0)
                printf("\tret void\n");
            else
                printf("\tret i32 0\n");
        }

        varNumber++;
    }
    else if(stmt->type == WHILE_T)
    {
        int thisWhileNum = whileNumber++;

        printf("\tbr label %%while.start%d\n\n", thisWhileNum);
        printf("while.start%d:\n", thisWhileNum);
        int exprVarNumber = genExpr(stmt->expr1).tempVarNum;
        printf("\tbr i1 %%%d, label %%while.do%d, label %%while.end%d\n\n", exprVarNumber, thisWhileNum, thisWhileNum);

        printf("while.do%d:\n", thisWhileNum);
        if(stmt->stmt1 != NULL)
            genStmt(stmt->stmt1);
        printf("\tbr label %%while.start%d\n", thisWhileNum);

        printf("\nwhile.end%d:\n", thisWhileNum);
    }
    else if(stmt->type == PRINT_T)
    {
        ExprRet ret = genExpr(stmt->expr1);
        if(ret.type == INT_T)
        {
            printf("\tcall i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @str.d, i32 0, i32 0), i32 %%%d)\n\n", ret.tempVarNum);
        }
        else if(ret.type == BOOL_T)
        {
            printf("\t%%%d = zext i1 %%%d to i32\n", varNumber++, ret.tempVarNum);
            printf("\t%%%d = getelementptr inbounds [2 x i8*]* @str.bools, i32 0, i32 %%%d\n", varNumber++, varNumber -1);
            printf("\t%%%d = load i8** %%%d\n", varNumber++, varNumber -1);
            printf("\tcall i32 (i8*, ...)* @printf(i8* %%%d)\n\n", varNumber -1);
        }
        varNumber++;
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

        printf("\tstore %s %%%d, %s* %s%s\n", llvmType, exprVarNumber, llvmType, varDeclSymbol, stmt->id);
    }
    else if(stmt->type == STOREARRAY)
    {
        int isLocal = 1;
        char varDeclSymbol[5];
        char llvmType1[MAX_LLVM_TYPE_SIZE];
        char llvmType2[MAX_LLVM_TYPE_SIZE];
        int indexVarNumber = genExpr(stmt->expr1).tempVarNum;
        int exprVarNumber = genExpr(stmt->expr2).tempVarNum;

        Type aux = -1;
        Type varType = getSymbolFromLocal(stmt->id);
        if(varType == aux)
        {
            isLocal = 0;
            varType = getSymbolFromGlobal(stmt->id);
        }

        sprintf(varDeclSymbol, isLocal ? "%%" : "@");
        getTypeLLVM(llvmType1, varType);

        Type type;
        if(varType == INTARRAY)
            type = INT_T;
        else if (varType == BOOLARRAY)
            type = BOOL_T;
        getTypeLLVM(llvmType2, type);

        printf("\t%%%d = load %s* %s%s\n", varNumber++, llvmType1, varDeclSymbol, stmt->id);

        printf("\t%%%d = extractvalue %s %%%d, 1\n", varNumber++, llvmType1, varNumber -1);
        printf("\t%%%d = getelementptr %s* %%%d, i32 %%%d\n", varNumber++, llvmType2, varNumber -1, indexVarNumber);
        printf("\tstore %s %%%d, %s* %%%d\n", llvmType2, exprVarNumber, llvmType2, varNumber -1);

    }
}

ExprRet buildExpression(Expr* expr, ExprRet leftExpr, ExprRet rightExpr, char* operation)
{
    ExprRet returnValue;
    char llvmType[MAX_LLVM_TYPE_SIZE];

    getTypeLLVM(llvmType, leftExpr.type);

    returnValue.tempVarNum = varNumber++;
    returnValue.type = leftExpr.type;

    printf("\t%%%d = %s %s %%%d, %%%d\n\n", returnValue.tempVarNum, operation, llvmType, leftExpr.tempVarNum, rightExpr.tempVarNum);

    return returnValue;
}

ExprRet genExpr(Expr* expr)
{
    ExprRet returnValue;
    ExprRet leftExprId, rightExprId;

    if(expr->type == BINOP)
    {
        if(expr->op == AND_T)
        {
            int thisAndNumber = andNumber++;

            //res = a
            int resVarNumber = varNumber++;
            printf("\t%%%d = alloca i1\n", resVarNumber);
            int leftExprIdNum = genExpr(expr->expr1).tempVarNum;
            printf("\tstore i1 %%%d, i1* %%%d\n", leftExprIdNum, resVarNumber);

            //if(a)
            printf("\t%%%d = icmp eq i1 1, %%%d\n", varNumber++, leftExprIdNum);
            printf("\tbr i1 %%%d, label %%and.do%d, label %%and.end%d\n\n", varNumber -1, thisAndNumber, thisAndNumber);

            //res = a
            printf("and.do%d:\n", thisAndNumber);
            int rightExprIdNum = genExpr(expr->expr2).tempVarNum;
            printf("\tstore i1 %%%d, i1* %%%d\n", rightExprIdNum, resVarNumber);
            printf("\tbr label %%and.end%d\n\n", thisAndNumber);

            printf("and.end%d:", thisAndNumber);
            printf("%%%d = load i1* %%%d\n", varNumber++, resVarNumber);

            returnValue.tempVarNum = varNumber -1;
            returnValue.type = BOOL_T;
        }
        else if(expr->op == OR_T)
        {
            int thisOrNumber = orNumber++;

            //res = a
            int resVarNumber = varNumber++;
            printf("\t%%%d = alloca i1\n", resVarNumber);
            int leftExprIdNum = genExpr(expr->expr1).tempVarNum;
            printf("\tstore i1 %%%d, i1* %%%d\n", leftExprIdNum, resVarNumber);

            //if(!a)
            printf("\t%%%d = icmp eq i1 0, %%%d\n", varNumber++, leftExprIdNum);
            printf("\tbr i1 %%%d, label %%or.do%d, label %%or.end%d\n\n", varNumber -1, thisOrNumber, thisOrNumber);

            //res = a
            printf("or.do%d:\n", thisOrNumber);
            int rightExprIdNum = genExpr(expr->expr2).tempVarNum;
            printf("\tstore i1 %%%d, i1* %%%d\n", rightExprIdNum, resVarNumber);
            printf("\tbr label %%or.end%d\n\n", thisOrNumber);

            printf("or.end%d:", thisOrNumber);
            printf("%%%d = load i1* %%%d\n", varNumber++, resVarNumber);

            returnValue.tempVarNum = varNumber -1;
            returnValue.type = BOOL_T;
        }
        else
        {
            char llvmOp[MAX_LLVM_OP_STRING];
            getOpLLVM(llvmOp, expr->op);

            leftExprId = genExpr(expr->expr1);
            rightExprId = genExpr(expr->expr2);
            returnValue = buildExpression(expr, leftExprId, rightExprId, llvmOp);

            if(expr->op != PLUS && expr->op != MINUS && expr->op != MUL && expr->op != DIV && expr->op != REM)
                returnValue.type = BOOL_T;
        }

        return returnValue;
    }
    else if(expr->type == UNOP)
    {
        ExprRet exprId;

        if(expr->op == PLUS)
        {
            exprId = genExpr(expr->expr1);
            returnValue.tempVarNum = varNumber++;
            printf("\t%%%d = add i32 0, %%%d\n\n", returnValue.tempVarNum, exprId.tempVarNum);
            returnValue.type = INT_T;
        }
        else if(expr->op == MINUS)
        {
            exprId = genExpr(expr->expr1);
            returnValue.tempVarNum = varNumber++;
            printf("\t%%%d = sub i32 0, %%%d\n\n", returnValue.tempVarNum, exprId.tempVarNum);
            returnValue.type = INT_T;
        }
        else if(expr->op == NOT)
        {
            int tempId;
            exprId = genExpr(expr->expr1);
            returnValue.tempVarNum = varNumber++;
            printf("\t%%%d = icmp ne i1 %%%d, 0\n", returnValue.tempVarNum, exprId.tempVarNum);
            printf("\t%%%d = xor i1 %%%d, true\n\n", varNumber, returnValue.tempVarNum);
            returnValue.tempVarNum = varNumber++;
            returnValue.type = BOOL_T;
        }
        else if(expr->op == DOTLENGTH_T)
        {
            leftExprId = genExpr(expr->expr1);

            if(leftExprId.type == STRINGARRAY)
            {
                printf("\t%%%d = sub i32 %%%s.len, 1\n", varNumber++, argCountName);
            }
            else
            {
                char llvmType[MAX_LLVM_TYPE_SIZE];
                getTypeLLVM(llvmType, leftExprId.type);

                printf("\t%%%d = extractvalue %s %%%d, 0\n", varNumber++, llvmType, varNumber -1);
            }

            returnValue.tempVarNum = varNumber -1;
            returnValue.type = INT_T;
        }
    }
    else if(expr->type == ID_T)
    {
        char llvmType[MAX_LLVM_TYPE_SIZE];
        Type idType = getSymbolFromLocal(expr->idOrLit);

        returnValue.tempVarNum = varNumber++;

        if(idType != -1)
        {
            getTypeLLVM(llvmType, idType);
            printf("\t%%%d = load %s* %%%s\n\n", returnValue.tempVarNum, llvmType, expr->idOrLit);
        }
        else
        {
            idType = getSymbolFromGlobal(expr->idOrLit);
            getTypeLLVM(llvmType, idType);
            printf("\t%%%d = load %s* @%s\n\n", returnValue.tempVarNum, llvmType, expr->idOrLit);
        }

        returnValue.type = idType;
    }
    else if(expr->type == INTLIT_T)
    {
        printf("\t%%%d = add i32 0, %d\n", varNumber++, (int) strtol(expr->idOrLit, NULL, 0));
        returnValue.tempVarNum = varNumber -1;
        returnValue.type = INT_T;
    }
    else if(expr->type == BOOLLIT_T)
    {
        if(strcmp(expr->idOrLit, "true") == 0) {
            printf("\t%%%d = add i1 0, 1\n", varNumber);
        }
        else {
            printf("\t%%%d = add i1 0, 0\n", varNumber);
        }        
        returnValue.tempVarNum = varNumber++;
        returnValue.type = BOOL_T;
    }
    else if(expr->type == CALL)
    {
        char llvmType[MAX_LLVM_TYPE_SIZE];
        Type methodType = getMethodFromGlobal(expr->idOrLit);
        getTypeLLVM(llvmType, methodType);

        //Count the amount of parameters
        int nParams = 0;
        ArgsList* aux = expr->argsList;
        for(; aux != NULL; aux = aux->next)
            nParams++;

        ExprRet* args = (ExprRet*) malloc(nParams * sizeof(ExprRet));

        int i;
        for(i=0, aux = expr->argsList; aux != NULL; aux = aux->next, i++)
            args[i] = genExpr(aux->expr);

        printf("\t%%%d = call %s @%s(", varNumber++, llvmType, expr->idOrLit);
        aux = expr->argsList;
        if(aux != NULL)
        {
            getTypeLLVM(llvmType, args[0].type);
            printf("%s %%%d", llvmType, args[0].tempVarNum);
            aux = aux->next;
        }
        for(i=1 ; aux != NULL; aux = aux->next, i++)
        {
            getTypeLLVM(llvmType, args[i].type);
            printf(", %s %%%d", llvmType, args[i].tempVarNum);
        }

        printf(")\n");

        free(args);

        returnValue.tempVarNum = varNumber -1;
        returnValue.type = methodType;
    }
    else if(expr->type == PARSEINT_T)
    {
        int indexVarNumber = genExpr(expr->expr1).tempVarNum;

        printf("\t%%%d = add i32 %%%d, 1\n", varNumber++, indexVarNumber);
        printf("\t%%%d = load i8*** %%%s\n", varNumber++, expr->idOrLit);
        printf("\t%%%d = getelementptr inbounds i8** %%%d, i32 %%%d\n", varNumber++, varNumber -1, varNumber -2);
        printf("\t%%%d = load i8** %%%d\n", varNumber++, varNumber -1);
        printf("\t%%%d = call i32 @atoi(i8* %%%d) nounwind readonly\n\n", varNumber++, varNumber -1);

        returnValue.tempVarNum = varNumber -1;
        returnValue.type = INT_T;
    }
    else if(expr->type == INDEX)
    {
        Type type;
        char llvmType1[MAX_LLVM_TYPE_SIZE];
        char llvmType2[MAX_LLVM_TYPE_SIZE];

        leftExprId = genExpr(expr->expr1);
        rightExprId = genExpr(expr->expr2);

        getTypeLLVM(llvmType1, leftExprId.type);

        if(leftExprId.type == INTARRAY)
            type = INT_T;
        else if(leftExprId.type == BOOLARRAY)
            type = BOOL_T;
        getTypeLLVM(llvmType2, type);

        printf("\t%%%d = extractvalue %s %%%d, 1\n", varNumber++, llvmType1, leftExprId.tempVarNum);
        printf("\t%%%d = getelementptr %s* %%%d, i32 %%%d\n", varNumber++, llvmType2, varNumber -1, rightExprId.tempVarNum);
        printf("\t%%%d = load %s* %%%d\n", varNumber++, llvmType2, varNumber -1);

        returnValue.tempVarNum = varNumber -1;
        returnValue.type = type;
    }
    else if(expr->type == NEWINTARR)
    {
        leftExprId = genExpr(expr->expr1);

        printf("\t%%%d = call noalias i8* @calloc(i32 %%%d, i32 4) nounwind\n\n", varNumber++, leftExprId.tempVarNum);
        printf("\t%%%d = bitcast i8* %%%d to i32*\n", varNumber++, varNumber -1);
        printf("\t%%%d = insertvalue %%Int.Array undef, i32 %%%d, 0\n", varNumber++, leftExprId.tempVarNum);
        printf("\t%%%d = insertvalue %%Int.Array %%%d, i32* %%%d, 1\n", varNumber++, varNumber -1, varNumber -2);

        returnValue.tempVarNum = varNumber -1;
        returnValue.type = INTARRAY;
    }
    else if(expr->type == NEWBOOLARR)
    {
        leftExprId = genExpr(expr->expr1);

        printf("\t%%%d = call noalias i8* @calloc(i32 %%%d, i32 1) nounwind\n\n", varNumber++, leftExprId.tempVarNum);
        printf("\t%%%d = bitcast i8* %%%d to i1*\n", varNumber++, varNumber -1);
        printf("\t%%%d = insertvalue %%Bool.Array undef, i32 %%%d, 0\n", varNumber++, leftExprId.tempVarNum);
        printf("\t%%%d = insertvalue %%Bool.Array %%%d, i1* %%%d, 1\n", varNumber++, varNumber -1, varNumber -2);

        returnValue.tempVarNum = varNumber -1;
        returnValue.type = BOOLARRAY;
    }

    return returnValue;
}

const char* llvmTypes[6] = {"void", "i32", "i1", "%Int.Array", "%Bool.Array", "i8**"};
void getTypeLLVM(char* llvmType, Type type)
{
    sprintf(llvmType, "%s", llvmTypes[type]);
}

const char* llvmOps[11] = {"add", "sub", "mul", "sdiv", "srem", "icmp slt", "icmp sgt", "icmp sle", "icmp sge", "icmp ne", "icmp eq"};
void getOpLLVM(char* llvmOp, OpType opType)
{
    sprintf(llvmOp, "%s", llvmOps[opType]);
}

