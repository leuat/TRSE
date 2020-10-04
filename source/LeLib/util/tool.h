#ifndef TOOL_H
#define TOOL_H

#include "source/Compiler/symboltable.h"
#include "spline2d.h"

class Tool
{
public:
    Tool();
    static bool AKGCompiler(QString filename, int address, SymbolTable* symTab);
    static void PathTool(QString svgFile, QString outBinary, int dataCount, float degreeFrame);
};

#endif // TOOL_H
