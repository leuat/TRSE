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

class MemoryBlock {
  public:
    int m_start, m_end, m_lineNumber;
    enum Type {CODE, DATA, MUSIC, USER, ARRAY};
    Type m_type;
    QVector<int> m_zeropages;
    QString m_name;

    MemoryBlock() {}
    MemoryBlock(int start, int end, Type type, QString name){
        m_start=start;
        m_end=end;
        m_type=type;
        m_name=name;
    }



    QString Type() {
        if (m_type==CODE) return "code";
        if (m_type==DATA) return "data";
        if (m_type==MUSIC) return "music";
        if (m_type==ARRAY) return "array";
        return "user";
    }
};



class Stack {
public:
    QVector<QString> m_vars;

    QString m_current;
    void push(QString s) {
        m_vars.push_back(s);
        m_current = s;
    }
    void pop() {
        m_current=m_vars[m_vars.count()-1];
        //m_vars.removeLast();
        m_vars.remove(m_vars.count()-1);
    }
  public:
        QString current() const;
};
class LabelStack {
public:
    QVector<QString> m_vars;
    QString m_current;
    QMap<QString, bool> sNumbersUsed;

    void push() {
        bool ok=false;
        while (!ok) {
            m_current = QString::number(rand()%100000);
            ok=true;
            if (sNumbersUsed.contains(m_current))
                ok = false;
        }
        sNumbersUsed[m_current] = true;

        m_vars.push_back(m_current);
    }
    void pop() {
        if (m_vars.count()==0) {
            ErrorHandler::e.Error("Trying to pop labelstack from zero : " + m_current);
        }
        m_vars.remove(m_vars.count()-1);
        if (m_vars.count()!=0)
            m_current = m_vars[m_vars.count()-1];

    }
};

class Appendix {
public:
    static int s_id;
    int m_id;
    Appendix() {
        m_id=s_id++;
    }
    Appendix(QString pos) {
        m_pos=pos;
        m_id=s_id++;
    }

    QStringList m_source;
    QString m_pos;

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
    QVector<Appendix> m_appendix;
    QVector<Appendix> m_extraBlocks;
    QVector<MemoryBlock> m_userWrittenBlocks;

    QString m_zeropageScreenMemory="$fb";
    QMap<QString, QString> m_replaceValues;

    QStringList m_internalZP;
    QVector<QString> m_zeroPointers; // org zp input
    QVector<QString> m_tempZeroPointers; // org temp zp input
    QVector<QString> m_zpStack; // temp zp stack

    Appendix* m_currentBlock = nullptr;

    void StartMemoryBlock(QString pos) {
        Appendix app(pos);
        m_appendix.append(app);
        m_currentBlock = &m_appendix[m_appendix.count()-1];
        m_currentBlock->Append("org "+pos,1);
    }

    void EndMemoryBlock() {
//        qDebug() << "Trying to end memory block.. ";
        if (m_currentBlock!=nullptr)
            Label("EndBlock"+QString::number(m_currentBlock->m_id));
        m_currentBlock=nullptr;
    }

    QString m_term;
    QMap<QString, Stack> m_stack;
    QMap<QString, LabelStack> m_labelStack;
    SymbolTable* m_symTab;
    QString m_projectDir;
    QVector<MemoryBlock*> blocks;

    QStringList m_tempVars;
    int m_varDeclEndsLineNumber = 0;
    int m_totalOptimizedLines;

    void SortAppendix();

    QMap<int, int> m_cycles;
    QMap<int, int> m_blockIndent;

    QVector<int> m_cycleCounter;
    QVector<int> m_blockCounter;

    void PushCounter();
    void PopCounter(int ln);

    void PushBlock(int ln);
    void PopBlock(int ln);


    virtual void InitZeroPointers(QStringList list1, QStringList list2) {

    }

    virtual int getLineCount() {return m_source.count();}
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

    QString getLabel(QString s) {
        return s+m_labelStack[s].m_current;
    }

    QString NewLabel(QString s) {
        m_labelStack[s].push();
        return s+m_labelStack[s].m_current;
    }

    void PopLabel(QString s) {
        m_labelStack[s].pop();
    }

    Assembler();
    void Save(QString filename);
    void Nl();
    void Write(QString str, int level=0);

    virtual void Program(QString name, QString vicParam) = 0;
    virtual void EndProgram() = 0;
    virtual void VarDeclHeader() = 0;
    virtual void DeclareVariable(QString name, QString type, QString initVal)=0;
    virtual void DeclareString(QString name, QStringList initVal) {}
    virtual void DeclareCString(QString name, QStringList initVal) {}
    virtual void DeclareArray(QString name, QString type, int count, QStringList lst, QString position) {}
    virtual void BeginBlock() = 0;
    virtual void EndBlock() = 0;
    virtual void AssignVariable(QString var) = 0;
    virtual void EndAssignVariable(QString var) {}
    virtual void ApplyTerm() = 0;
    virtual void Number(QString n) = 0;
    virtual QString  String(QStringList s) = 0;
    virtual void BinOP(TokenType::Type t)=0;
    virtual void Poke(bool start) = 0;
    virtual void Peek(bool start) {}
    virtual void Term(QString s, bool write=false);
    virtual void Comment(QString s) {}
    void Term();
    virtual void Writeln() = 0;
    virtual void EndWriteln() = 0;
    virtual void WriteBuiltinWriteln() {}
    virtual void StartPrint() {}
    virtual void Variable(QString s, bool isByte) {}
    //void Appendix(QString s, int l);
    virtual void LoadVariable(QString var) {}
    void Connect();
    virtual QString StoreInTempVar(QString name, QString type="byte")  { return name;}
    virtual void PopTempVar() {}
    virtual void StartForLoop(QString a, QString b) {}
    virtual void EndForLoop(QString endVal) {}
    void Asm(QString s, QString comment="");
    void Label(QString s);
    virtual void Optimise(CIniFile& ini) {}

    virtual void IncludeFile(QString file) {}
    void ClearTerm() {
        m_term = "";
    }
};

#endif // ASSEMBLER_H
