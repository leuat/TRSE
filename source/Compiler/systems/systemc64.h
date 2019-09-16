#ifndef SYSTEMC64_H
#define SYSTEMC64_H

#include "systemmos6502.h"

class SystemC64 : public SystemMOS6502
{
public:
    SystemC64(CIniFile* settings, CIniFile* proj);
};


class SystemX16 : public SystemMOS6502
{
public:
    SystemX16(CIniFile* settings, CIniFile* proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = X16;
        m_systemColor = QColor(100,60,40);

    }
};

#endif // SYSTEMC64_H
