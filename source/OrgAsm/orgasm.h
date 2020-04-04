#pragma once

#include <QString>
#include <QStringList>
#include <QTime>
#include "source/LeLib/util/util.h"
#include "source/Compiler/syntax.h"
#include "source/Compiler/Opcodes/opcodes6502.h"


class OrgasmData {
public:
    enum PassType { PASS_LABELS, PASS_SYMBOLS, PASS_COMPILE };


    static QString ReplaceWord(QString& line, QString& word, QString replacement);
    static QString BinopExpr(QString& expr, int& val, QString rep);

};


class OrgasmInstruction {
    public:
        QString m_opCode;
        enum Type {none, imm, zp, zpx, zpy, izx, izy, abs, abx,aby, ind, rel, label, imp};
        Type m_type;
        int m_size = 0;
//        enum Pass { passSymbol, passCompile };


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
            int i = 0;
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

/*        QByteArray Assemble(QString& expr, Opcodes6502 &m_opCodes,
                            OrgasmData::PassType pass, QMap<QString, int>& symbols,
                            int pCounter, QMap<QString, QString>& constants, QMap<QString, QRegExp*>& exp,
                            QVector<QString>& symList, QVector<QString>& constList);

*/
    };



class OrgasmLine {
public:
    enum OLType {INSTRUCTION, ORG, INCBIN, CONSTANT, BYTE, WORD, LABELONLY, PADBYTE};

    OLType m_type;
    int m_pos, m_lineNumber=0;
    QString m_expr="";
    QString m_label="";
    QString m_orgLine="";
    OrgasmInstruction m_instruction;
    bool m_ignore = false;

    static bool m_inIFDEF;


    QString Type() {
        if (m_type==INSTRUCTION)
            return "INSTRUCTION";
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
    }

};


class OrgasmError {
public:
    QString msg;
    OrgasmLine oline;
    OrgasmError(QString s, OrgasmLine ln) {
        msg = s;
        oline = ln;
    }
};



class Orgasm {
public:
    QString m_source;
    QStringList m_lines;
    QString m_currentChar;
    QVector<OrgasmLine> m_ol;
    QVector<QString> m_symbolsList, m_constList;
    QMap<QString, int> m_symbols;
    QMap<QString, QString> m_constants;
    Opcodes6502 m_opCodes;
    QVector<OrgasmLine> m_olines;
    bool m_success = false;
    int m_constantPassLines;
    QByteArray m_data;
    QMap<QString, QRegExp*> m_regs;
    QString m_output;



    int m_pCounter = 0;
    bool m_done = false;
    void LoadFile(QString filename);

    void LoadCodes();
    void ProcessSource();


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
    void ProcessInstructionData(OrgasmLine& ol, OrgasmData::PassType pd);
    void SaveSymbolsList(QString dupFile);
};
