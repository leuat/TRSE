#ifndef ORGASM68k_H
#define ORGAsm68k_H


#include "orgasm.h"
/*
 *  The unfinished M6809 assembler!
 *  (hopefully)
 *
*/

class Opm68k {
public:
    Opm68k() {

    }
    unsigned short opcode;
    int size;
    QString name;
    QString pattern;

};


class OrgAsm68k : public Orgasm
{


public:
    OrgAsm68k();
    void ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd) override;
    QStringList m_regs = QStringList() << "d0"<<"d1"<<"d2"<<"d3"<<"d4"<<"d5" <<"d6"<<"d7"<<"a0"<<"a1"<<"a2"<<"a3"<<"a4"<<"a5"<<"a6"<<"a7";
    QStringList m_ignoreCommands = QStringList() << "processor" <<"cpu" <<"org" << "end";
    bool isRegister(QString str) {
        return m_regs.contains(str.toLower());
    }


    QMap<QString,int> m_opcodes;
    uchar getRegisterCodeFromParams(QString s);
    uchar getPushPullParams(OrgasmLine& ol);
    int getTypeFromParams(OrgasmLine& ol);
    int getParsedValue(OrgasmLine& ol, int& size, int type);
    int getParsedInt(QString p1);
    void LoadCodes(int CPUflavor) override;
    QMap<QString,QSharedPointer<Opm68k>> m_instructions;
    QString m_opCode;
    Opm68k* getOpcode(OrgasmLine&ol);
    void Write(QByteArray& d, OrgasmLine&l, int type);
    void WriteNumber(QByteArray&d, int val, int type);

};

#endif // OrgAsm68k_H
