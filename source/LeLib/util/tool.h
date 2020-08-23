#ifndef TOOL_H
#define TOOL_H

#include "source/Compiler/symboltable.h"


class Tool
{
public:
    Tool();
    static void AKGCompiler(QString filename, int address, SymbolTable* symTab);
};

#endif // TOOL_H
