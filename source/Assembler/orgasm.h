#ifndef ORGASM_H
#define ORGASM_H
#include <QFile>
#include <QString>
#include <QStringList>

#include "source/Compiler/Opcodes/opcodes6502.h"
#include "source/LeLib/util/util.h"
class Instruction {
public:
    QString m_line;
    QString m_opCode;
    QString m_label;
    enum Type {imm, zp, zpx, zpy, izx, izy, abs, abx,aby, ind, rel, label, imp, none};
    QStringList m_debug;
    Type m_type;
    QString m_arg;
    int m_size = 0;
    QByteArray m_data;

    Type getType(QString s) {
        s=s.toLower();
        qDebug() << " Getting type from : " << s;
        if (s=="")
            return none;
        if (s.startsWith("#"))
            return imm;
        int i = 0;
        if (s.contains(","))
            i = Util::NumberFromStringHex(s.split(",")[0]);
        else
            i = Util::NumberFromStringHex(s);


        if (!s.contains("(") && s.contains(",x"))
            return zpx;
        if (!s.contains("(") && s.contains(",y"))
            return zpy;
        if (s.contains("(") && s.contains(",x)"))
            return izx;
        if (s.contains("(") && s.contains("),y"))
            return izy;
        if (!s.contains("(") &&i>=256 && !s.contains(")")) {
            return abs;

        }
        if (!s.contains("(")&&i>=256 && !s.contains(")") && s.contains(",x"))
            return abx;
        if (!s.contains("(")&&i>=256 && !s.contains(")") && s.contains(",y"))
            return aby;
        if (s.contains("(")&&i>=256 && s.contains(")") && !s.contains(","))
            return ind;


        return none;
    }


    bool Assemble(QString line, Opcodes6502& m_opCodes, int pCounter);

};


class OrgAsm
{
public:
    OrgAsm();
    QStringList m_debug;
    QString m_output;
    Opcodes6502 m_opCodes;
    QByteArray m_data;



    void LoadCodes();
    int m_pCounter=0;

    QMap<QString, int> m_symbols;

    void Debug(QString s);
    QString TreatLabel(QString s);

    void TreatOrg(QStringList lst);
    void TreatData(QString s);
    void Assemble(QString inFile, QString outFile);

};

#endif // ORGASM_H
