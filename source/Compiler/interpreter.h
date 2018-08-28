#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "source/Compiler/assembler/mos6502.h"
#include "source/Compiler/assembler/asmpascal.h"

class Interpreter
{
public:
    Node* m_tree = nullptr;
    enum Type{PASCAL, MOS6502};
    Assembler* m_assembler = nullptr;
    Parser* m_parser;
    FatalErrorException recentError;
    Interpreter(Parser* p);
    Interpreter() {}
    void Parse();
    void Visit(Node* n);
    void Interpret();
    bool Build(Type, QString projDir, QStringList zp, bool optimize);
    void CleanupCycleLinenumbers();
    void CleanupBlockLinenumbers();
    void SaveBuild(QString filename);
    void HandleError(FatalErrorException fe, QString se);
    void FindLineNumberAndFile(int inLe, QString& file, int& outle);
};

#endif // INTERPRETER_H
