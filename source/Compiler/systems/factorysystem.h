#ifndef FACTORYSYSTEM_H
#define FACTORYSYSTEM_H

#include "systemmos6502.h"
#include "systemc64.h"
#include "systemc128.h"
#include "systempet.h"
#include "systemvic20.h"
#include "systemnes.h"
#include "systemamiga.h"
#include "source/LeLib/util/cinifile.h"

class FactorySystem
{
public:
    FactorySystem();

    static AbstractSystem* Create(AbstractSystem::System type,CIniFile* settings, CIniFile* proj) {
        AbstractSystem* s = nullptr;
        if (type==AbstractSystem::C64)
            return new SystemC64(settings, proj);
        if (type==AbstractSystem::C128)
            return new SystemC128(settings, proj);
        if (type==AbstractSystem::PET)
            return new SystemPET(settings, proj);
        if (type==AbstractSystem::VIC20)
            return new SystemVIC20(settings, proj);
        if (type==AbstractSystem::NES)
            return new SystemNES(settings, proj);
        if (type==AbstractSystem::AMIGA)
            return new SystemAmiga(settings, proj);

        return s;
    }


};

#endif // FACTORYSYSTEM_H
