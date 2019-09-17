#ifndef SYSTEMNES_H
#define SYSTEMNES_H

#include "systemmos6502.h"

class SystemNES : public SystemMOS6502
{
public:
    SystemNES(CIniFile* settings, CIniFile* proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = NES;
        m_startAddress = 0xCA00;
        m_programStartAddress = 0xCA00;
    }
};

#endif // SYSTEMNES_H
