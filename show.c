#include <stdio.h>
#include "show.h"

void printDeclList(DeclList*);

void printProgram(Class* myProgram)
{
    printf("Program\n");
    printf("\tId(%s)\n", myProgram->id);

    DeclList* aux = myProgram->declList;
    for(; aux->next != NULL; aux = aux->next)
        printDeclList(aux);
}

void printDeclList(DeclList* list)
{

}
