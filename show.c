#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "show.h"

////
// Utilities
void typeToString(Type, char*);
void typeToStringST(Type, char*);

////
// Abstract Syntax Tree
void printDeclList(DeclList*);
void printFieldDecl(VarDecl*);
void printMethodDecl(MethodDecl*);
void printMethodParams(ParamList*);
void printVarDeclList(VarDeclList*);
void printStmtList(StmtList*, int);
void printStmt(Stmt*, int);
void printExpr(Expr*, int);
void printOp(OpType, ExprType, int);

void printProgram(Class* myProgram)
{
    if(!myProgram)
        return;

    printf("Program\n");
    printf("  Id(%s)\n", myProgram->id);

    if(myProgram->declList != NULL)
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
    char type[TYPE_SIZE];

    typeToString(decl->type, type);
    printf("  VarDecl\n    %s\n", type);

    IDList* aux = decl->idList;
    for(; aux != NULL; aux = aux->next)
        printf("    Id(%s)\n", aux->id);
}

void printMethodDecl(MethodDecl* decl)
{
    char type[TYPE_SIZE];
    typeToString(decl->type, type);

    printf("  MethodDecl\n");
    printf("    %s\n", type);
    printf("    Id(%s)\n", decl->id);
    printMethodParams(decl->paramList);
    printf("    MethodBody\n");
    printVarDeclList(decl->varDeclList);
    printStmtList(decl->stmtList, 3);
}

void printMethodParams(ParamList* params)
{
    printf("    MethodParams\n");

    char type[TYPE_SIZE];
    ParamList* aux = params;
    for(; aux != NULL; aux = aux->next)
    {
        printf("      ParamDeclaration\n");
        typeToString(aux->type, type);
        printf("        %s\n", type);
        printf("        Id(%s)\n", aux->id);
    }
}

void printVarDeclList(VarDeclList* decls)
{
    char type[TYPE_SIZE];
    VarDeclList* aux = decls;

    for(; aux != NULL; aux = aux->next)
    {
        typeToString(aux->varDecl->type, type);
        printf("      VarDecl\n");
        printf("        %s\n", type);

        IDList* aux2 = aux->varDecl->idList;
        for(; aux2 != NULL; aux2 = aux2->next)
        {
            printf("        Id(%s)\n", aux2->id);
        }
    }
}

void printStmtList(StmtList* stmts, int depth)
{
    StmtList* aux = stmts;
    for(; aux != NULL; aux = aux->next)
        printStmt(aux->stmt, depth);
}

void printStmt(Stmt* stmt, int depth)
{
    int newDepth = depth +1;
    char* tabs = (char*) calloc(2*depth+1, sizeof(char));
    memset(tabs, ' ', 2*depth * sizeof(char));

    if(stmt == NULL)
    {
        printf("%sNull\n", tabs);
        return;
    }

    if(stmt->type == CSTAT)
    {
        printf("%sCompoundStat\n", tabs);
        printStmtList(stmt->stmtList, newDepth);
    }
    else if(stmt->type == IFELSE)
    {
        printf("%sIfElse\n", tabs);
        printExpr(stmt->expr1, newDepth);
        printStmt(stmt->stmt1, newDepth);
        printStmt(stmt->stmt2, newDepth);
    }
    else if(stmt->type == RETURN_T)
    {
        printf("%sReturn\n", tabs);
        if(stmt->expr1 != NULL)
            printExpr(stmt->expr1, newDepth);
    }
    else if(stmt->type == WHILE_T)
    {
        printf("%sWhile\n", tabs);
        printExpr(stmt->expr1, newDepth);
        printStmt(stmt->stmt1, newDepth);
    }
    else if(stmt->type == PRINT_T)
    {
        printf("%sPrint\n", tabs);
        printExpr(stmt->expr1, newDepth);
    }
    else if(stmt->type == STORE)
    {
        printf("%sStore\n", tabs);
        printf("%s  Id(%s)\n", tabs, stmt->id);
        printExpr(stmt->expr1, newDepth);
    }
    else if(stmt->type == STOREARRAY)
    {
        printf("%sStoreArray\n", tabs);
        printf("%s  Id(%s)\n", tabs, stmt->id);
        printExpr(stmt->expr1, newDepth);
        printExpr(stmt->expr2, newDepth);
    }

    free(tabs);
}

void printExpr(Expr* expr, int depth)
{
    int newDepth = depth +1;
    char* tabs = (char*) calloc(2*depth+1, sizeof(char));
    memset(tabs, ' ', 2*depth * sizeof(char));

    if(expr->type == BINOP)
    {
        printOp(expr->op, expr->type, depth);
        printExpr(expr->expr1, newDepth);
        printExpr(expr->expr2, newDepth);
    }
    else if(expr->type == UNOP)
    {
        printOp(expr->op, expr->type, depth);
        printExpr(expr->expr1, newDepth);
    }
    else if(expr->type == ID_T)
        printf("%sId(%s)\n", tabs, expr->idOrLit);
    else if(expr->type == INTLIT_T)
        printf("%sIntLit(%s)\n", tabs, expr->idOrLit);
    else if(expr->type == BOOLLIT_T)
        printf("%sBoolLit(%s)\n", tabs, expr->idOrLit);
    else if(expr->type == CALL)
    {
        printf("%sCall\n", tabs);
        printf("%s  Id(%s)\n", tabs, expr->idOrLit);

        ArgsList* aux = expr->argsList;
        for(; aux != NULL; aux = aux->next)
            printExpr(aux->expr, newDepth);
    }
    else if(expr->type == PARSEINT_T)
    {
        printf("%sParseArgs\n", tabs);
        printf("%s  Id(%s)\n", tabs, expr->idOrLit);
        printExpr(expr->expr1, newDepth);
    }
    else if(expr->type == INDEX)
    {
        printf("%sLoadArray\n", tabs);
        printExpr(expr->expr1, newDepth);
        printExpr(expr->expr2, newDepth);
    }
    else if(expr->type == NEWINTARR)
    {
        printf("%sNewInt\n", tabs);
        printExpr(expr->expr1, newDepth);
    }
    else if(expr->type == NEWBOOLARR)
    {
        printf("%sNewBool\n", tabs);
        printExpr(expr->expr1, newDepth);
    }

    free(tabs);
}

void printOp(OpType op, ExprType exprType, int depth)
{
    char* tabs = (char*) calloc(2*depth+1, sizeof(char));
    memset(tabs, ' ', 2*depth * sizeof(char));

    if(op == OR_T)
        printf("%sOr\n", tabs);
    else if(op == AND_T)
        printf("%sAnd\n", tabs);
    else if(op == EQ)
        printf("%sEq\n", tabs);
    else if(op == DIF)
        printf("%sNeq\n", tabs);
    else if(op == LESSER)
        printf("%sLt\n", tabs);
    else if(op == GREATER)
        printf("%sGt\n", tabs);
    else if(op == LEQ)
        printf("%sLeq\n", tabs);
    else if(op == GEQ)
        printf("%sGeq\n", tabs);
    else if(op == PLUS)
    {
        if(exprType == BINOP)
            printf("%sAdd\n", tabs);
        else
            printf("%sPlus\n", tabs);
    }
    else if(op == MINUS)
    {
        if(exprType == BINOP)
            printf("%sSub\n", tabs);
        else
            printf("%sMinus\n", tabs);
    }
    else if(op == MUL)
        printf("%sMul\n", tabs);
    else if(op == DIV)
        printf("%sDiv\n", tabs);
    else if(op == REM)
        printf("%sMod\n", tabs);
    else if(op == NOT)
        printf("%sNot\n", tabs);
    else if(op == DOTLENGTH_T)
        printf("%sLength\n", tabs);

    free(tabs);
}


////
// Symbol Tables
void printMethodTable(ClassTableEntry*);

void printSymbolTables(ClassTable* table)
{
    printf("===== Class %s Symbol Table =====\n", table->id);

    ////
    // Class Symbol Table.
    char type[TYPE_SIZE];
    ClassTableEntry* aux = table->entries;
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->methodTable == NULL) //Variable
        {
            typeToStringST(aux->type, type);
            printf("%s\t%s\n", aux->id, type);
        }
        else //Method
            printf("%s\tmethod\n", aux->id);
    }


    ////
    // Method Symbol Tables.
    aux = table->entries;
    for(; aux != NULL; aux = aux->next)
    {
        if(aux->methodTable != NULL) //Method
            printMethodTable(aux);
    }
}

void printMethodTable(ClassTableEntry* table)
{
    char type[TYPE_SIZE];
    typeToStringST(table->type, type);

    printf("\n===== Method %s Symbol Table =====\n", table->id);
    printf("return\t%s\n", type);

    MethodTableEntry* aux = table->methodTable->entries;
    for(; aux != NULL; aux = aux->next)
    {
        typeToStringST(aux->type, type);
        printf("%s\t%s", aux->id, type);
        if(aux->isParam)
            printf("\tparam");
        printf("\n");
    }
}

////
// Utilities.
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

void typeToStringST(Type type, char* dest)
{
    if(type == INT_T)
        sprintf(dest, "%s", "int");
    else if(type == BOOL_T)
        sprintf(dest, "%s", "boolean");
    else if(type == INTARRAY)
        sprintf(dest, "%s", "int[]");
    else if(type == BOOLARRAY)
        sprintf(dest, "%s", "boolean[]");
    else if(type == VOID_T)
        sprintf(dest, "%s", "void");
    else if(type == STRINGARRAY)
        sprintf(dest, "%s", "String[]");
}
