#ifndef SYSTEMNES_H
#define SYSTEMNES_H

#include "systemmos6502.h"

class SystemNES : public SystemMOS6502
{
public:
    SystemNES(CIniFile* settings, CIniFile* proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = NES;
    }
};

#endif // SYSTEMNES_H
