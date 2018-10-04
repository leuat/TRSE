#ifndef Compiler_H
#define Compiler_H

#include "parser.h"
#include "source/Compiler/assembler/mos6502.h"
#include "source/Compiler/assembler/asmpascal.h"
#include "source/LeLib/util/cinifile.h"

class Compiler
{
public:
    Node* m_tree = nullptr;
    enum Type{PASCAL, MOS6502};
    Assembler* m_assembler = nullptr;
    Parser* m_parser;
    FatalErrorException recentError;
    Compiler(Parser* p);
    Compiler() {}
    void Parse();
    void Visit(Node* n);
    void Interpret();
    bool Build(Type, QString projDir, CIniFile& ini);
    void CleanupCycleLinenumbers();
    void CleanupBlockLinenumbers();
    void SaveBuild(QString filename);
    void HandleError(FatalErrorException fe, QString se);
    void FindLineNumberAndFile(int inLe, QString& file, int& outle);
};

#endif // Compiler_H
