#ifndef ZORGASM_H
#define ZORGASM_H


#include "orgasm.h"
/*
 *  The finished Z80 assembler!
 *  (hopefully)
 *
*/

class ZOrgasm : public Orgasm
{
public:
    ZOrgasm();
    void ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd) override;
    QStringList m_regs = QStringList() << "r"<<"i"<<"a" <<"b"<<"c"<<"d"<<"af"<<"bc"<<"hl"<<"de"<< "z" <<"h" <<"l"<<"e"<<"ix"<<"iy"<<"sp"<<"af'"<<"ixh"<<"iyh"<<"ixl"<<"iyl" <<"nz" << "nc" <<"po"<<"pe"<<"m" <<"p";
    QStringList m_16bitRegs = QStringList() << "af"<<"bc"<<"hl"<<"de"<<"ix"<<"iy"<<"sp"<<"af";
    QStringList m_8bitRegs = QStringList() << "r"<<"i"<<"a" <<"b"<<"c"<<"d"<<"z" <<"h" <<"l"<<"e"<<"ixh"<<"iyh"<<"ixl"<<"iyl" <<"nz" << "nc" <<"po"<<"pe" <<"m"<<"p";
    QStringList m_ignoreCommands = QStringList() << "processor" <<"cpu" <<"org" << "end";
    bool isRegister(QString str) {
        return m_regs.contains(str.toLower());
    };
    void LoadCodes(int CPUflavor) override;
    QMap<QString,int> m_opCodes;
    QString m_opCode;

    QString WashForOpcode(QString s, QString& value,OrgasmLine& ol);

    QString Process(QString s, OrgasmLine& ol);
};

#endif // ZORGASM_H
