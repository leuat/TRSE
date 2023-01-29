#ifndef FACTORYMETHODS_H
#define FACTORYMETHODS_H

#include <QSharedPointer>
#include "methods6502.h"
#include "methods6502vic20.h"
#include "methods6502c64.h"
#include "methods6502ok64.h"
#include "methods68000.h"
#include "methods6800amiga.h"
#include "methods68000atari.h"
#include "methodsz80.h"
#include "methodsx86.h"
#include "methods6809.h"

class FactoryMethods
{
public:
    FactoryMethods();

    static QSharedPointer<AbstractMethods> CreateMethods(AbstractSystem::System s);
};

#endif // FACTORYMETHODS_H
