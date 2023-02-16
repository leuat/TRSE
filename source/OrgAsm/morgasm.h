#ifndef MORGASM_H
#define MORGASM_H


#include "orgasm.h"
/*
 *  The unfinished M6809 assembler!
 *  (hopefully)
 *
*/

class Op6809 {
public:
    Op6809() {

    }
    QVector<int> opcodes;
    static const int imm = 0;
    static const int dir = 1;
    static const int ind = 2;
    static const int ext = 3;
    static const int inh = 4;
    QString name;

};


class MOrgasm : public Orgasm
{


public:
    MOrgasm();
    void ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd) override;
    QStringList m_regs = QStringList() << "a"<<"b"<<"d"<<"x"<<"y"<<"u" <<"s";
    QStringList m_16bitRegs = QStringList() << "x"<<"u"<<"y"<<"s";
    QStringList m_8bitRegs = QStringList() << "a"<<"b";
    QStringList m_ignoreCommands = QStringList() << "processor" <<"cpu" <<"org" << "end";
    bool isRegister(QString str) {
        return m_regs.contains(str.toLower());
    }
    int getTypeFromParams(QString s);
    void LoadCodes(int CPUflavor) override;
    QMap<QString,QSharedPointer<Op6809>> m_instructions;
    QString m_opCode;

    QString WashForOpcode(QString s, QString& value,OrgasmLine& ol);

    QString Process(QString s, OrgasmLine& ol);
};

#endif // MORGASM_H
