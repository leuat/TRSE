#ifndef ZORGASM_H
#define ZORGASM_H


#include "orgasm.h"
/*
 *  The Unfinished Z80 assembler!
 *
*/

class ZOrgasm : public Orgasm
{
public:
    ZOrgasm();
    void ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd) override;
    QStringList m_regs = QStringList() << "a" <<"b"<<"c"<<"d"<<"af"<<"bc"<<"hl"<<"de"<< "z" <<"h" <<"l"<<"e"<<"ix"<<"iy"<<"sp"<<"af'"<<"ixh"<<"iyh"<<"ixl"<<"iyl";
    QStringList m_16bitRegs = QStringList() << "af"<<"bc"<<"hl"<<"de"<<"ix"<<"iy"<<"sp"<<"af'";
    QStringList m_8bitRegs = QStringList() << "a" <<"b"<<"c"<<"d"<<"z" <<"h" <<"l"<<"e"<<"ixh"<<"iyh"<<"ixl"<<"iyl";
    QStringList m_ignoreCommands = QStringList() << "processor" <<"cpu" <<"org" << "end";
    bool isRegister(QString str) {
        return m_regs.contains(str.toLower());
    };
    void LoadCodes(int CPUflavor) override;
    QMap<QString,int> m_opCodes;
    QString m_opCode;

    QString WashForOpcode(QString s, QString& value);

    QString Process(QString s);
};

#endif // ZORGASM_H
