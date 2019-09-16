#ifndef SYSTEMVIC20_H
#define SYSTEMVIC20_H

#include "systemmos6502.h"

class SystemVIC20 : public SystemMOS6502
{
public:
    SystemVIC20(CIniFile* settings, CIniFile* proj);
};

#endif // SYSTEMVIC20_H
