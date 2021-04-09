#ifndef SYSTEMC128_H
#define SYSTEMC128_H

#include "systemmos6502.h"

class SystemC128 : public SystemMOS6502
{
public:
    SystemC128(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = C128;
        m_systemColor = QColor(20,100,80);
        m_startAddress =getDefaultBasicAddress();
        m_programStartAddress = 0x1C10;

    }
    int getDefaultBasicAddress() override {
        return 0x1C01;
    };
    bool isCommodoreSystem() override  {return true;}

};
#endif // SYSTEMC128_H
