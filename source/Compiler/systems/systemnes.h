#ifndef SYSTEMNES_H
#define SYSTEMNES_H

#include "systemmos6502.h"

class SystemNES : public SystemMOS6502
{
public:
    SystemNES(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = NES;
        DefaultValues();
    }
    void DefaultValues() override {
        m_startAddress = 0xCA00;
        m_programStartAddress = 0xCA00;
        m_ignoreSys = false;
        m_stripPrg = false;

    }

};

#endif // SYSTEMNES_H
