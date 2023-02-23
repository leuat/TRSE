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
    QVector<int> size;
    static const int imm = 0;
    static const int dir = 1;
    static const int ind = 2;
    static const int ext = 3;
    static const int inh = 4;
    static const int rel = 5;
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

    QMap<QString,int> m_codeReg;
    QMap<QString,int> m_pushPullReg;
    QMap<QString,int> m_lea;
    QMap<QString,int> m_lda;
    uchar getRegisterCodeFromParams(QString s);
    uchar getPushPullParams(OrgasmLine& ol);
    int getLeaParams(OrgasmLine& ol, int&size);
    int getLdaParams(OrgasmLine& ol, int&size);
    int getTypeFromParams(OrgasmLine& ol);
    int getParsedValue(OrgasmLine& ol, int& size, int type);
    void LoadCodes(int CPUflavor) override;
    QMap<QString,QSharedPointer<Op6809>> m_instructions;
    QString m_opCode;
    Op6809* getOpcode(OrgasmLine&ol);
    void Write(QByteArray& d, OrgasmLine&l, int type);
    void WriteNumber(QByteArray&d, int val, int type);

};

#endif // MORGASM_H
