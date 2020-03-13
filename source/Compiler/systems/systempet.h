#ifndef SYSTEMPET_H
#define SYSTEMPET_H

#include "systemmos6502.h"

class SystemPET : public SystemMOS6502
{
public:
    SystemPET(CIniFile* settings, CIniFile* proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = PET;
        DefaultValues();
    }
    void DefaultValues() override {
        m_startAddress = 0x0400;
        m_programStartAddress = 0x0410;
        m_ignoreSys = false;
        m_stripPrg = false;

    }

};


#endif // SYSTEMPET_H
