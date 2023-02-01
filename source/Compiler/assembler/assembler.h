/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <QString>
#include <QVector>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include "source/Compiler/token.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/symboltable.h"
#include "source/LeLib/util/cinifile.h"
#include "source/Compiler/optimiser/postoptimiser.h"
#include "source/LeLib/util/utilclasses.h"

class MemoryBlock {
  public:
    int m_start, m_end, m_lineNumber, m_bank=-1;
    enum Type {CODE, DATA, MUSIC, USER, ARRAY};
    Type m_type = CODE;
    bool m_isOverlapping = false;
    int m_shift = 0;


    QVector<int> m_zeropages;
    QString m_name;
    int m_currentLineNumber;
    MemoryBlock() {}
    MemoryBlock(int start, int end, Type type, QString name){
        m_start=start;
        m_end=end;
        m_type=type;
        m_name=name;
    }
    ~MemoryBlock();



    QString Type() {
        if (m_type==CODE) return "code";
        if (m_type==DATA) return "data";
        if (m_type==MUSIC) return "music";
        if (m_type==ARRAY) return "array";
        return "user";
    }
};





class LabelStack {
public:
    QVector<QString> m_vars;
    QString m_current;
    static int m_labelCount;

    QMap<QString, bool> sNumbersUsed;
    void push();
    void pop();
};

class Appendix {
public:
    static int s_id;
    int m_id;
    int m_dataSize = 0;
    int m_incDataSize = 0;
    Appendix() {
        m_id=s_id++;
    }
    bool m_extraOutput = false;
    Appendix(QString pos) {
        m_pos=pos;
        m_id=s_id++;
    }
    bool m_isMainBlock = false;
    QStringList m_source;
    QString m_pos;

    QStringList getSource();



    void Append(QString str, int level)
    {
        QString s ="";
        for (int i=0;i<level;i++)
            s+="\t";
        s+=str;
        m_source.append(s);
    }

};


class Assembler
{
public:
    QStringList m_source;
    QVector<QSharedPointer<Appendix>> m_appendix;
    QSharedPointer<Appendix> m_currentBlock = nullptr;
    QSharedPointer<Appendix> m_tempVarsBlock = nullptr;
    QSharedPointer<Appendix> m_mainBlock = nullptr;
    QVector<QSharedPointer<Appendix>> m_blockStack;
    QVector<Appendix> m_extraBlocks;
    QSharedPointer<Appendix> m_chipMem;
    QSharedPointer<Appendix> m_hram;
    QSharedPointer<Appendix> m_wram, m_sprram, m_ram;
    QMap<QString,QSharedPointer<Appendix>> m_banks;
    QMap<QString, QString> m_lastRegister; // Last registers set
    QString m_insertEndBlock = "";





    QSharedPointer<PostOptimiser> m_optimiser = nullptr;

    bool m_disableComments = false;
    bool m_countCycles = false;
    int m_noBanks = 0;
    static int m_prevCycles;

    QVector<int> m_removeLines;


    void StartExistingBlock(QSharedPointer<Appendix> block);
    void EndCurrentBlock();

    QString m_currentBlockName="";

    QVector<MemoryBlock> m_userWrittenBlocks;
    QStringList m_startInsertAssembler;
    QStringList m_endInsertAssembler;
    QString m_zeropageScreenMemory="$fe";
    QString m_zeropageColorMemory="$fc";
    QMap<QString, QString> m_replaceValues;

    RegisterStack m_regAcc;
    RegisterStack m_regMem;
    Stack m_varStack;
    bool m_isTheRealEnd = false;
    QString m_lblFailed, m_lblSuccess;
    QString m_curDir;
    int m_currentBreakpoint = 0;

//    static QStringList m_internalZP;
    static RegisterStack m_internalZP;
    QVector<QString> m_zeroPointers; // org zp input
    QVector<QString> m_tempZeroPointers; // org temp zp input
    QVector<QString> m_zpStack; // temp zp stack

    QMap<QString, QString> m_defines;
    // Labels for hi/lo integers
    QString ilo = "_i_lo";
    QString ihi = "_i_hi";






    int m_zbyte = 0x80;

    virtual void StartMemoryBlock(QString pos);

    virtual void EndMemoryBlock();

    QString m_term;
    QMap<QString, Stack> m_stack;
    QMap<QString, LabelStack> m_labelStack;
    QSharedPointer<SymbolTable> m_symTab;
    QString m_projectDir;
    QVector<QSharedPointer<MemoryBlock>> blocks, userBlocks;

    QStringList m_tempVars;
    int m_varDeclEndsLineNumber = 0;
    int m_totalOptimizedLines = 0;

    bool m_ignoreInitialJump = false;

    void SortAppendix();

    // Override call (label) etc
    virtual QString jumpLabel(QString lbl) {return lbl; }

    QMap<int, int> m_cycles, m_blockCycles;
    QMap<int, int> m_cyclesOut, m_blockCyclesOut;
    QMap<int,int> m_addressesOut, m_addresses;

    QMap<int, int> m_blockIndent;

    QVector<int> m_cycleCounter;
    QVector<int> m_blockCounter;

    void PushCounter();
    void PopCounter(int ln);

    void PushBlock(int ln);
    void PopBlock(int ln);

    virtual void DeclareInternalVariable(QString name) {}

    virtual int CodeSizeEstimator(QStringList& lines) = 0;


    virtual void InitZeroPointers(QStringList list1, QStringList list2, QStringList list3) {

    }

    QStringList getSource();

    virtual int getLineCount();
    int CountCycles(QString s);
    virtual int CountInstructionCycle(QStringList s) {return 0;}

    virtual QString PushZeroPointer() { return ""; }
    virtual void PopZeroPointer() {}
    virtual void PopAllZeroPointers() {}

    void VarDeclEnds() {
        if (m_varDeclEndsLineNumber == 0) // Only assign if not previously
            m_varDeclEndsLineNumber = m_source.count();
    }
    virtual bool CheckZPAvailability() {return false;}

    void Delete();

    QString getLabel(QString s);
    QString m_hash = "#";
    virtual QString NewLabel(QString s);

    virtual void PopLabel(QString s);
    QString byte = "dc.b";
    QString word = "dc.w";
    QString llong = "dc.l";
    QString ppointer = "dc.w";

    QString getLabelEnding(QString name);

    Assembler();
    virtual ~Assembler();
    void Save(QString filename);
    void Nl();
    virtual void Write(QString str, int level=0);




    virtual QString GetOrg(int pos ) = 0;
    virtual QString GetOrg();



    virtual void Program(QString name, QString vicParam) {};
    virtual void EndProgram() {}
  //  virtual void VarDeclHeader() {}
    virtual void DeclareVariable(QString name, QString type, QString initVal, QString position){}
    virtual void DeclareString(QString name, QStringList initVal, QStringList flags) {}
    virtual void DeclareCString(QString name, QStringList initVal, QStringList flags) {}
    virtual void DeclareArray(QString name, QString type, int count, QStringList lst, QString position) {}
    virtual void BeginBlock() {}
    virtual void EndBlock() {}
//    virtual void AssignVariable(QString var) = 0;
//    virtual void EndAssignVariable(QString var) {}
    virtual void ApplyTerm() {}
    virtual void Number(QString n) {}
    virtual QString  String(QStringList s, bool term) { return "";}
    virtual void BinOP(TokenType::Type t, bool clearFlag=true){}
    virtual void BinOP16(TokenType::Type t, bool clearFlag=true){}
 //   virtual void BinOP(TokenType::Type t, bool clearFlag=true){ BinOP(t,false,true);}
//    virtual void Poke(bool start) = 0;
//    virtual void Peek(bool start) {}
    virtual void Term(QString s, bool write=false);
    virtual void Comment(QString s);
    void Term();
  //  virtual void Writeln() = 0;
  //  virtual void EndWriteln() = 0;
  //  virtual void WriteBuiltinWriteln() {}
  //  virtual void StartPrint() {}

    virtual void Variable(QString s, bool isByte) {}
    //void Appendix(QString s, int l);
    virtual void LoadVariable(QString var) {}

    virtual void Connect();
    virtual QString StoreInTempVar(QString name, QString type="byte", bool actuallyStore=true);
    virtual void PopTempVar() {}
//    virtual void StartForLoop(QString a, QString b) {}
//    virtual void EndForLoop(QString endVal) {}

    void Asm(QString s, QString comment="");
    virtual void Label(QString s);
    virtual void Optimise(CIniFile& ini) {}

    virtual void IncludeFile(QString pfile, bool isInsert =false);


    virtual bool DeclareRecord(QString name, QString type, int count, QStringList data, QString pos);
    virtual bool DeclareClass(QString name, QString type, int count, QStringList data, QString pos);


    virtual void RemoveLines();

    virtual void RemoveLinesDebug();

    virtual void RemoveUnusedLabels();


    QString getLine(int i);

    QString getNextLine(int i, int &j);

    bool nextLineIsLabel(int i);

    QString getToken(QString s, int t);


    void WriteConstants();

    void ClearTerm() {
        m_term = "";
    }

    /**
     * Special optimization stuff
     *
    **/

};

#endif // ASSEMBLER_H
