#ifndef SYSTEMC128_H
#define SYSTEMC128_H

#include "systemmos6502.h"

class SystemC128 : public SystemMOS6502
{
public:
    SystemC128(CIniFile* settings, CIniFile* proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = C128;
        m_systemColor = QColor(20,100,80);
    }
};
#endif // SYSTEMC128_H
