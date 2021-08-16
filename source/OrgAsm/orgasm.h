#pragma once

#include <QString>
#include <QStringList>
#include <QTimer>
#include "source/LeLib/util/util.h"
#include "source/Compiler/syntax.h"
#include "source/Compiler/opcodes/opcodes6502.h"
#include "source/Compiler/symboltable.h"
#include <QSharedPointer>


class OrgasmData {
public:
    enum PassType { PASS_LABELS, PASS_SYMBOLS, PASS_COMPILE };


    static QString ReplaceWord(QString& line, QString& word, QString replacement);
    static QString BinopExpr(QString& expr, long& val, QString rep);

};


class OrgasmInstruction {
    public:
        QString m_opCode;
        enum Type {none, imm, zp, zpx, zpy, izx, izy,abs, abx,aby, ind, rel, izz, label, imp};
        Type m_type;
        int m_size = 0;
//        enum Pass { passSymbol, passCompile };
        bool m_bracketsAroundVariables = false;

        void Init(QStringList lst) {
            m_opCode = lst[0];
//            m_expr = lst[1];
            if (lst.count()>1)
                m_type = getTypeFromParams(lst[1]);
            else m_type=none;
        }

        Type getTypeFromParams(QString s) {
            s=s.toLower();
            //        qDebug() << " Getting type from : " << s;
            if (s=="")
                return none;
            if (s.startsWith("#"))
                return imm;
            long i = 0;
            bool ok;
            if (s.contains(",")) {
                ok = Util::NumberFromStringHex(s.split(",")[0],i);
            }
            else
            {
                ok = Util::NumberFromStringHex(s,i);
            }

            if (!ok)
                i  = 0x1000; // Force using address

            if (i<256 && !s.contains(","))
                return zp;

            if (!s.contains("(") && i<256 && s.contains(",x"))
                return zpx;
            if (!s.contains("(")  && i<256 && s.contains(",y"))
                return zpy;
            if (s.contains("(") && s.contains(",x)")) {
                return izx;
            }
            if (s.contains("(") && s.contains("),y"))
                return izy;
            if (s.contains("(") && s.contains("),z"))
                return izz;
            if (!s.contains("(") && i>=256 && !s.contains(")") && !s.contains(",x") && !s.contains(",y")) {

                return abs;

            }
            if (!s.contains("(")&&i>=256 && !s.contains(")") && s.contains(",x")) {
                return abx;
            }
            if (!s.contains("(")&&i>=256 && !s.contains(")") && s.contains(",y"))
                return aby;
            if (s.contains("(")&&i>=256 && s.contains(")") && !s.contains(","))
                return ind;


            return none;
        }


        Type getTypeFromParamsZ80(QString s);

/*        QByteArray Assemble(QString& expr, Opcodes6502 &m_opCodes,
                            OrgasmData::PassType pass, QMap<QString, int>& symbols,
                            int pCounter, QMap<QString, QString>& constants, QMap<QString, QRegExp*>& exp,
                            QVector<QString>& symList, QVector<QString>& constList);

*/
    };



class OrgasmLine {
public:
    enum OLType {INSTRUCTION, ORG, INCBIN, CONSTANT, BYTE, WORD, LABELONLY, PADBYTE, ORGASMCOMMAND};

    OLType m_type;
    int m_pos, m_lineNumber, m_rasLineNumber=0;
    QString m_expr="";
    QString m_label="";
    QString m_orgLine="";
    OrgasmInstruction m_instruction;
    bool m_ignore = false;
    static bool m_inIFDEF;

    QString Type() {
        if (m_type==INSTRUCTION)
            return "INSTRUCTION";
        if (m_type==ORGASMCOMMAND)
            return "ORGASMCOMMAND";
        if (m_type==ORG)
            return "ORG";
        if (m_type==INCBIN)
            return "INCBIN";
        if (m_type==CONSTANT)
            return "CONSTANT";
        if (m_type==BYTE)
            return "BYTE";
        if (m_type==CONSTANT)
            return "WORD";
        if (m_type==LABELONLY)
            return "LABELONLY";
        return "";
    }

};


class OrgasmError {
public:
    QString msg = "";
    OrgasmLine oline;
    OrgasmError(QString s, OrgasmLine ln) {
        msg = s;
        oline = ln;
    }
    OrgasmError(QString s, int line) {
        msg = s;
        oline.m_lineNumber = line;

    }
    OrgasmError() {
        msg = "";
    }
};



class Orgasm  : public QObject{
    Q_OBJECT
public:

    QStringList m_extraMonCommands;
    int m_curRasLine = 0;
    QString m_source;
    QStringList m_lines;
    QString m_currentChar;
    QVector<OrgasmLine> m_ol;
    QVector<QString> m_symbolsList, m_constList;
    QMap<QString, QString> m_extraSymbols; // Variables declared AT
    QMap<QString, int> m_symbols;
    QMap<QString, QString> m_constants;
    Opcodes6502 m_opCodes;
    QVector<OrgasmLine> m_olines;
    bool m_success = false;
    int m_constantPassLines;
    int m_hasOverlappingError = false;
    OrgasmData::PassType m_passType;
    QByteArray m_data;
    QMap<QString, QRegularExpression*> m_regs;
    QMap<int,int> m_lineAddress;
    QString m_output;
    QString m_prevLabel = "";

    QString processRepeatIndex(QString s, int current);
    QStringList processRepeatIndex(QStringList s, int current);

    QString endl = "\n";
    OrgasmError error;

    void SetupConstants(QSharedPointer<SymbolTable> symTab) {
        if (symTab == nullptr)
            return;
        for(QString k: symTab->m_constants.keys()) {
            m_constants[k] = Util::numToHex(symTab->m_constants[k]->m_value->m_fVal);
        }

    }


    int m_pCounter = 0;
    bool m_done = false;
    void LoadFile(QString filename);

    static const int CPUFLAVOR_6502_STOCK = 0;
    static const int CPUFLAVOR_6502_ILLEGAL = 1;
    static const int CPUFLAVOR_GS4510 = 2;
    static const int CPUFLAVOR_Z80 = 3;

    int m_cpuFlavor = CPUFLAVOR_6502_STOCK;

    virtual void LoadCodes(int CPUflavor);
    void ProcessSource();
    void ProcessUnrolling();


    OrgasmLine LexLine(int i);

    bool Assemble(QString filename, QString out);

    void PassFindConstants();
    void PassReplaceConstants();
    void PassLabels();
    void Compile(OrgasmData::PassType pt);

    void ProcessByteData(OrgasmLine& ol,OrgasmData::PassType );
    void ProcessBytePad(OrgasmLine& ol);
    void ProcessWordData(OrgasmLine& ol);
    void ProcessOrgData(OrgasmLine& ol);
    void ProcessIncBin(OrgasmLine& ol);
    virtual void ProcessInstructionData(OrgasmLine& ol, OrgasmData::PassType pd);
    void SaveSymbolsList(QString dupFile);
signals:
    void EmitTick(QString val);
};
