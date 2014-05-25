#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "astNodes.h"
#include "show.h"
#include "symbols.h"
#include "semantic.h"
#include "exitClean.h"

char* currentMethod;
MethodTable* currentLocalTable;
extern ClassTable* symbolsTable;

char* opsOrig[] = {"+", "-", "*", "/", "%", "<", ">", "<=", ">=", "!=", "==", "!", ".length", "&&", "||"};

////
// Semantic error checking functions.
void checkMethodDecl(MethodDecl*);
void checkStmtList(StmtList*);
void checkStmt(Stmt*);
Type checkExpr(Expr*);
int isValidIntLit(char*);

////
// Error functions.
void errorInvalidLiteral(char*);
void errorIncompatibleTypeInStatement(char*, Type, Type);
void errorIncompatibleTypeInStatement2(char*, Type, Type, Type);
void errorOperatorCannotBeApplied(char*, Type);
void errorOperatorCannotBeApplied2(char*, Type, Type);
void errorIncompatibleTypeInAssign(char*, Type, Type);
void errorIncompatibleTypeInAssignArray(char*, Type, Type);
void errorCannotFindSymbol(char*);
void errorIncompatibleArgument(int, char*, Type, Type);

void checkSemantics(Class* myProgram)
{
    DeclList* aux = myProgram->declList;
    for(; aux != NULL; aux = aux->next)
        if(aux->type == METHODDECL)
        {
            currentMethod = aux->methodDecl->id;
            currentLocalTable = getLocalTable(currentMethod);
            checkStmtList(aux->methodDecl->stmtList);
        }
}

void checkStmtList(StmtList* stmtList)
{
    StmtList* aux = stmtList;
    for(; aux != NULL; aux = aux->next)
        checkStmt(aux->stmt);
}

void checkStmt(Stmt* stmt)
{
    if(!stmt)
        return;

    Type t1, t2, t3;

    if(stmt->type == CSTAT)
        checkStmtList(stmt->stmtList);
    else if(stmt->type == IFELSE)
    {
        t1 = checkExpr(stmt->expr1);
        if(t1 != BOOL_T) //ERROR
            errorIncompatibleTypeInStatement("if", t1, BOOL_T);

        checkStmt(stmt->stmt1);
        checkStmt(stmt->stmt2);
    }
    else if(stmt->type == WHILE_T)
    {
        t1 = checkExpr(stmt->expr1);
        if(t1 != BOOL_T) //ERROR
            errorIncompatibleTypeInStatement("while", t1, BOOL_T);

        checkStmt(stmt->stmt1);
    }
    else if(stmt->type == RETURN_T)
    {
        t1 = getMethodFromGlobal(currentMethod);
        if(stmt->expr1)
            t2 = checkExpr(stmt->expr1);
        else
            t2 = VOID_T;
        if(t1 != t2) //ERROR
            errorIncompatibleTypeInStatement("return", t2, t1);
    }
    else if(stmt->type == PRINT_T)
    {
        t1 = checkExpr(stmt->expr1);
        if(t1 != BOOL_T && t1 != INT_T) //ERROR
            errorIncompatibleTypeInStatement2("System.out.println", t1, BOOL_T, INT_T);
    }
    else if(stmt->type == STORE)
    {
        t1 = getSymbolFromLocalOrGlobal(stmt->id);
        t2 = -1;
        if(t1 == t2)
            errorCannotFindSymbol(stmt->id);

        t2 = checkExpr(stmt->expr1);
        if(t1 != t2)
            errorIncompatibleTypeInAssign(stmt->id, t2, t1);
    }
    else if(stmt->type == STOREARRAY)
    {
        t1 = getSymbolFromLocalOrGlobal(stmt->id);
        t2 = -1;
        if(t1 == t2)
            errorCannotFindSymbol(stmt->id);

        t2 = checkExpr(stmt->expr1);
        if((t1 != INTARRAY && t1 != BOOLARRAY) || t2 != INT_T) //ERROR
            errorOperatorCannotBeApplied2("[", t1, t2);

        if(t1 == INTARRAY)
            t3 = INT_T;
        else
            t3 = BOOL_T;

        t2 = checkExpr(stmt->expr2);
        if(t3 != t2) //ERROR
            errorIncompatibleTypeInAssignArray(stmt->id, t2, t3);
    }
}

Type checkExpr(Expr* expr)
{
    Type t1, t2;

    if(expr->type == BINOP)
    {
        switch(expr->op)
        {
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case REM:
            t1 = checkExpr(expr->expr1);
            t2 = checkExpr(expr->expr2);
            if(t1 != INT_T || t2 != INT_T) //ERROR
                errorOperatorCannotBeApplied2(opsOrig[expr->op], t1, t2);
            return INT_T;
        case LESSER:
        case GREATER:
        case LEQ:
        case GEQ:
            t1 = checkExpr(expr->expr1);
            t2 = checkExpr(expr->expr2);
            if(t1 != INT_T || t2 != INT_T) //ERROR
                errorOperatorCannotBeApplied2(opsOrig[expr->op], t1, t2);
            return BOOL_T;
        case DIF:
        case EQ:
            t1 = checkExpr(expr->expr1);
            t2 = checkExpr(expr->expr2);
            if(t1 != t2) //ERROR
                errorOperatorCannotBeApplied2(opsOrig[expr->op], t1, t2);
            return BOOL_T;
        case AND_T:
        case OR_T:
            t1 = checkExpr(expr->expr1);
            t2 = checkExpr(expr->expr2);
            if(t1 != BOOL_T || t2 != BOOL_T) //ERROR
                errorOperatorCannotBeApplied2(opsOrig[expr->op], t1, t2);
            return BOOL_T;
        }
    }
    else if(expr->type == UNOP)
    {
        if(expr->op == PLUS || expr->op == MINUS)
        {
            t1 = checkExpr(expr->expr1);
            if(t1 != INT_T) //ERROR
                errorOperatorCannotBeApplied(opsOrig[expr->op], t1);
            return INT_T;
        }
        else if(expr->op == NOT)
        {
            t1 = checkExpr(expr->expr1);
            if(t1 != BOOL_T) //ERROR
                errorOperatorCannotBeApplied(opsOrig[expr->op], t1);
            return BOOL_T;
        }
        else if(expr->op == DOTLENGTH_T)
        {
            t1 = checkExpr(expr->expr1);
            if(t1 != INTARRAY && t1 != BOOLARRAY && t1 != STRINGARRAY) //ERROR
                errorOperatorCannotBeApplied(opsOrig[expr->op], t1);
            return INT_T;
        }
    }
    else if(expr->type == ID_T)
    {
        Type r, notFound = -1;
        r = getSymbolFromLocalOrGlobal(expr->idOrLit);
        if(r == notFound)
            errorCannotFindSymbol(expr->idOrLit);
        return r;
    }
    else if(expr->type == INTLIT_T)
    {
        if(!isValidIntLit(expr->idOrLit)) //ERROR
            errorInvalidLiteral(expr->idOrLit);

        return INT_T;
    }
    else if(expr->type == BOOLLIT_T)
        return BOOL_T;
    else if(expr->type == CALL)
    {
        MethodTable* t = getLocalTable(expr->idOrLit);
        if(t == NULL)
            errorCannotFindSymbol(expr->idOrLit);

        int argNum = 0;
        ArgsList* auxArgs = expr->argsList;
        MethodTableEntry* auxParams = t->entries;
        while(auxArgs != NULL || auxParams != NULL)
        {
            t1 = VOID_T;
            if(auxArgs != NULL)
            {
                t1 = checkExpr(auxArgs->expr);
                auxArgs = auxArgs->next;
            }

            t2 = VOID_T;
            if(auxParams != NULL)
            {
                if(auxParams->isParam)
                    t2 = auxParams->type;
                auxParams = auxParams->next;
                if(auxParams && !auxParams->isParam)
                    auxParams = NULL;
            }

            if(t1 != t2) //ERROR
                errorIncompatibleArgument(argNum, expr->idOrLit, t1, t2);

            argNum++;
        }

        return getMethodFromGlobal(expr->idOrLit);
    }
    else if(expr->type == PARSEINT_T)
    {
        t1 = getSymbolFromLocalOrGlobal(expr->idOrLit);
        t2 = -1;
        if(t1 == t2)
            errorCannotFindSymbol(expr->idOrLit);

        t2 = checkExpr(expr->expr1);
        if(t1 != STRINGARRAY || t2 != INT_T) //ERROR
            errorOperatorCannotBeApplied2("Integer.parseInt", t1, t2);

        return INT_T;
    }
    else if(expr->type == INDEX)
    {
        t1 = checkExpr(expr->expr1);
        t2 = checkExpr(expr->expr2);
        if((t1 != INTARRAY && t1 != BOOLARRAY) || t2 != INT_T) //ERROR
            errorOperatorCannotBeApplied2("[", t1, t2);

        if(t1 == INTARRAY)
            return INT_T;
        else
            return BOOL_T;
    }
    else if(expr->type == NEWINTARR)
    {
        t1 = checkExpr(expr->expr1);
        if(t1 != INT_T)
            errorOperatorCannotBeApplied("new int", t1);
        return INTARRAY;
    }
    else if(expr->type == NEWBOOLARR)
    {
        t1 = checkExpr(expr->expr1);
        if(t1 != INT_T)
            errorOperatorCannotBeApplied("new boolean", t1);
        return BOOLARRAY;
    }
}

int isValidIntLit(char* intlit)
{
    int len = strlen(intlit);
    if(intlit[0] != '0')
        return 1;

    if(len >= 1 && intlit[0] == '0' && intlit[1] == 'x')
        return 1;

    int i;
    for(i=1; i < len; i++)
        if(intlit[i] == '9' || intlit[i] == '8')
            return 0;

    return 1;
}

////
// Error functions.
void errorInvalidLiteral(char* lit)
{
    printf("Invalid literal %s\n", lit);
    exitClean(0);
}

void errorIncompatibleTypeInStatement(char* stmt, Type t, Type r)
{
    char typeT[TYPE_SIZE], typeR[TYPE_SIZE];
    typeToStringST(t, typeT);
    typeToStringST(r, typeR);

    printf("Incompatible type in %s statement (got %s, required %s)\n", stmt, typeT, typeR);

    exitClean(0);
}

void errorIncompatibleTypeInStatement2(char* stmt, Type g, Type r1, Type r2)
{
    char typeG[TYPE_SIZE], typeR1[TYPE_SIZE], typeR2[TYPE_SIZE];
    typeToStringST(g, typeG);
    typeToStringST(r1, typeR1);
    typeToStringST(r2, typeR2);

    printf("Incompatible type in %s statement (got %s, required %s or %s)\n", stmt, typeG, typeR1, typeR2);

    exitClean(0);
}

void errorOperatorCannotBeApplied(char* op, Type t)
{
    char type[TYPE_SIZE];
    typeToStringST(t, type);

    printf("Operator %s cannot be applied to type %s\n", op, type);

    exitClean(0);
}

void errorOperatorCannotBeApplied2(char* op, Type t1, Type t2)
{
    char type1[TYPE_SIZE], type2[TYPE_SIZE];
    typeToStringST(t1, type1);
    typeToStringST(t2, type2);

    printf("Operator %s cannot be applied to types %s, %s\n", op, type1, type2);

    exitClean(0);
}

void errorIncompatibleTypeInAssign(char* id, Type got, Type req)
{
    char typeGot[TYPE_SIZE], typeReq[TYPE_SIZE];
    typeToStringST(got, typeGot);
    typeToStringST(req, typeReq);

    printf("Incompatible type in assignment to %s (got %s, required %s)\n", id, typeGot, typeReq);

    exitClean(0);
}

void errorIncompatibleTypeInAssignArray(char* id, Type got, Type req)
{
    char typeGot[TYPE_SIZE], typeReq[TYPE_SIZE];
    typeToStringST(got, typeGot);
    typeToStringST(req, typeReq);

    printf("Incompatible type in assignment to %s[] (got %s, required %s)\n", id, typeGot, typeReq);

    exitClean(0);
}

void errorIncompatibleArgument(int argNum, char* id, Type g, Type r)
{
    char typeG[TYPE_SIZE], typeR[TYPE_SIZE];
    typeToStringST(g, typeG);
    typeToStringST(r, typeR);

    printf("Incompatible type of argument %d in call to method %s (got %s, required %s)\n", argNum, id, typeG, typeR);

    exitClean(0);
}

void errorCannotFindSymbol(char* id)
{
    printf("Cannot find symbol %s\n", id);

    exitClean(0);
}
