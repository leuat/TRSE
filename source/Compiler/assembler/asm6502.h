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

#ifndef MOS6502_H
#define MOS6502_H

#include "source/Compiler/assembler/assembler.h"
#include "source/LeLib/util/util.h"
#include "source/LeLib/util/cinifile.h"
#include "source/Compiler/opcodes/opcodes6502.h"

class CStringItem {
public:
    uchar m_pscii, m_screenCode;
    QString m_char;
    CStringItem(QString ch, uchar pscii, uchar screenCode) {
        m_char=ch;
        m_pscii = pscii;
        m_screenCode = screenCode;
    }
    CStringItem() {

    }
};


class Asm6502 : public Assembler
{
public:
    Asm6502();
    ~Asm6502() override;

    bool m_hasOpenBlock = false;
    static QString m_defaultZeroPointers;
    static QString m_defaultViaZeroPointers;
    static QString m_defaultTempZeroPointers;

    QStringList m_varZeroPointers;

    QMap<QString, CStringItem> m_cstr;

    Opcodes6502 m_opCycles;

    int m_curZeroPointer=0;

    QVector<int> m_removeLines;
    bool endTerm() {
        if (m_term.split(" ").count()==2)
            return true;
        return false;
    }
    QString m_currentVar="";
//    QString m_currentLoopVar="";
    //int m_lblIdx = 0;
    QMap<QString, int> m_lblIdx;

    QString intToHexString(int val);


    void InitCStrings();

//    bool DeclareRecord(QString name, QString type, int count, QStringList data, QString pos);

    void Program(QString name, QString vicParams) override;
    void EndProgram() override;
    void DeclareArray(QString name, QString type, int count, QStringList lst, QString pos) override;
    void InitZeroPointers(QStringList lst, QStringList tmpList, QStringList varList) override;

    void VarDeclHeader();
    void DeclareVariable(QString name, QString type, QString initval, QString position) override;
    void DeclareString(QString name, QStringList initval, QStringList flags) override;
    void DeclareCString(QString name, QStringList initval, QStringList flags) override;

    void PopTempVar() override;
    void BeginBlock() override;
    void EndBlock() override;
    void ApplyTerm() override;
    void Number(QString n) override;
    void Comment(QString s) override;
    QString String(QStringList s, bool term) override;
 //   void AssignVariable(QString v) override;
 //   void EndAssignVariable(QString variable) override;
    void BinOP(TokenType::Type t, bool clearFlag=true) override;
 //   void Poke(bool start) override;
 //   void Peek(bool start) override;

    QString PushZeroPointer() override;
    void PopZeroPointer() override;
    void PopAllZeroPointers() override { m_curZeroPointer=0;}
    bool CheckZPAvailability() override;

    QString GetOrg(int pos);
    void DeclareInternalVariable(QString name) override;

    int CodeSizeEstimator(QStringList& lines) override;

    QString StoreInTempVar(QString name, QString type="byte") override;


//    void Writeln() override;
 //   void EndWriteln() override;
    void LoadVariable(QString var) override;

    void Variable(QString var, bool isBtyte) override;


   // void WriteBuiltinWriteln() override;
   // void StartPrint() override;


  //  void StartForLoop(QString a, QString b) override;
  //  void EndForLoop(QString endVal) override;

    void Optimise(CIniFile& ini) override;
    void OptimisePassStaLda();
    void OptimisePassStaLda2();

    void OptimisePassLdx(QString x);
    void OptimisePassLdaTax(QString x);
    void OptimiseJumps();
    void OptimiseCmp(QString op);
    void OptimisePhaPla1();
    void OptimisePhaPla2();
    void OptimisePhaLdxPla();
    void OptimiseLdLd();
    void OptimisePassStaLdx(QString x);
    void OptimisePassLdyLdy(QString y);

    QString getLine(int i);
    QString getNextLine(int i, int &j);
    bool nextLineIsLabel(int i);
    QString getToken(QString, int t);
    void RemoveLines();
    void RemoveLinesDebug();

    bool ContainsAChangingOpcodes(QString l1);
    bool ContainsYUsingOpcodes(QString l1,QString y);
    bool ContainsYChangingOpcodes(QString l1,QString y);
    bool ContainsBranches(QString l1);




    MOSOperation GetOperand(QStringList s);
    int CalculateCycles(MOSOperation op);
    int CountInstructionCycle(QStringList s) override;
};



#endif // MOS6502_H
