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



class MOSOperandCycle {
public:
    QString m_name;
    int m_implied;
  //  int m_acc;
    int m_immediate;
    int m_absolute;
    int m_absoluteWithParam;
    int m_zeropage;
    int m_zeropageWithParam;
    MOSOperandCycle() {}
    MOSOperandCycle(QString name, int implied, int immediate, int absolute, int abswp, int zp, int zpwp) {
        m_name = name;
        m_implied = implied;
 //       m_acc = acc;
        m_immediate = immediate;
        m_absolute = absolute;
        m_absoluteWithParam = abswp;
        m_zeropage = zp;
        m_zeropageWithParam = zpwp;
    }
};

class MOSOperation {
  public:
    QString operand;
    QString param1 = "";
    QString param2 ="";
    bool isNumeric = false;
    bool is16bit = false;
    bool isZeroPage = false;
};


class AsmMOS6502 : public Assembler
{
public:
    AsmMOS6502();

    QMap<QString, MOSOperandCycle> m_opCycles;

    static QString m_defaultZeroPointers;
    QMap<QString, CStringItem> m_cstr;


    QVector<QString> m_zeroPointers;
    int m_curZeroPointer=0;

    QVector<int> m_removeLines;
    bool endTerm() {
        if (m_term.split(" ").count()==2)
            return true;
        return false;
    }
    QString byte = "dc.b";
    QString word = "dc.w";
    QString m_currentVar="";
//    QString m_currentLoopVar="";
    //int m_lblIdx = 0;
    QMap<QString, int> m_lblIdx;


    void InitCStrings();


    void Program(QString name) override;
    void EndProgram() override;
    void DeclareArray(QString name, QString type, int count, QStringList lst, QString pos) override;
    void InitMosOpCycles();
    void InitZeroPointers(QStringList lst) override;

    void VarDeclHeader();
    void DeclareVariable(QString name, QString type, QString initval) override;
    void DeclareString(QString name, QStringList initval) override;
    void DeclareCString(QString name, QStringList initval) override;

    void BeginBlock() override;
    void EndBlock() override;
    void ApplyTerm() override;
    void Number(QString n) override;
    void Comment(QString s) override;
    QString String(QStringList s) override;
    void AssignVariable(QString v) override;
    void EndAssignVariable(QString variable) override;
    void BinOP(TokenType::Type t) override;
    void Poke(bool start) override;
    void Peek(bool start) override;

    QString PushZeroPointer() override;
    void PopZeroPointer() override;
    void PopAllZeroPointers() override { m_curZeroPointer=0;}
    bool CheckZPAvailability() override;



    QString StoreInTempVar(QString name, QString type="byte") override;


    void Writeln() override;
    void EndWriteln() override;
    void LoadVariable(QString var) override;

    void Variable(QString var, bool isBtyte) override;


    void WriteBuiltinWriteln() override;
    void StartPrint() override;

    void IncludeFile(QString file) override;

    void StartForLoop(QString a, QString b) override;
    void EndForLoop(QString endVal) override;

    void Optimise() override;
    void OptimisePassStaLda();
    void OptimisePassLdx(QString x);
    void OptimisePassLdaTax(QString x);
    void OptimiseJumps();
    QString getLine(int i);
    QString getNextLine(int i, int &j);
    QString getToken(QString, int t);
    void RemoveLines();

    int getLineCount() override;


    MOSOperation GetOperand(QStringList s);
    int CalculateCycles(MOSOperation op);
    int CountInstructionCycle(QStringList s) override;
};


#endif // MOS6502_H
