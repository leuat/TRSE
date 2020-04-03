#ifndef FACTORYMETHODS_H
#define FACTORYMETHODS_H

#include <QSharedPointer>
#include "mos6502/methods6502.h"
#include "mos6502/methods6502vic20.h"
#include "mos6502/methods6502c64.h"
#include "mos6502/methods6502ok64.h"
#include "methods68000.h"
#include "methodsx86.h"

class FactoryMethods
{
public:
    FactoryMethods();

    static QSharedPointer<AbstractMethods> CreateMethods(AbstractSystem::System s);
};

#endif // FACTORYMETHODS_H
