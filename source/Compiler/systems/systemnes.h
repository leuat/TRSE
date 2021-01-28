#ifndef SYSTEMNES_H
#define SYSTEMNES_H

#include "systemmos6502.h"

class SystemNES : public SystemMOS6502
{
public:
    SystemNES(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void DefaultValues() override;
};

#endif // SYSTEMNES_H
