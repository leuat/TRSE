#ifndef SYSTEMATARI2600_H
#define SYSTEMATARI2600_H

#include "systemmos6502.h"

class SystemAtari2600 : public SystemMOS6502
{
public:
    SystemAtari2600(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void DefaultValues() override;
};


#endif // SYSTEMATARI2600_H
