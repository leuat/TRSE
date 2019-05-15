#ifndef SYSTEMVIC20_H
#define SYSTEMVIC20_H

#include "systemmos6502.h"

class SystemVIC20 : public SystemMOS6502
{
public:
    SystemVIC20(CIniFile* settings, CIniFile* proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = VIC20;
    }
};

#endif // SYSTEMVIC20_H
