#ifndef SYSTEMC64_H
#define SYSTEMC64_H

#include "systemmos6502.h"

class SystemC64 : public SystemMOS6502
{
public:
    SystemC64(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) ;
    void DefaultValues() override;

    int getDefaultBasicAddress() override {
        return 0x801;
    };
    bool isCommodoreSystem() override  {return true;}


};


class SystemX16 : public SystemMOS6502
{
public:
    SystemX16(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = X16;
        m_systemColor = QColor(100,60,40);

    }
};

#endif // SYSTEMC64_H
