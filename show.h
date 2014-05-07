#ifndef SHOW_H
#define SHOW_H

#include "astNodes.h"
#include "symbols.h"

#define TYPE_SIZE 20

void printProgram(Class*);
void printSymbolTables(ClassTable*);

void typeToStringST(Type, char*);

#endif
