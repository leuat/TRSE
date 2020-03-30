#ifndef SYSTEMVIC20_H
#define SYSTEMVIC20_H

#include "systemmos6502.h"

class SystemVIC20 : public SystemMOS6502
{
public:
    QString param;
    SystemVIC20(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void DefaultValues();

};

#endif // SYSTEMVIC20_H
