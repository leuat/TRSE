#ifndef FACTORYSYSTEM_H
#define FACTORYSYSTEM_H

#include "systemmos6502.h"
#include "systemc64.h"
#include "systemc128.h"
#include "systempet.h"
#include "systemvic20.h"
#include "systemnes.h"
#include "systemamiga.h"
#include "systemplus4.h"
#include "systemok64.h"
#include "systemx86.h"
#include "systemgameboy.h"
#include "systemspectrum.h"
#include "systemtiki100.h"
#include "systematari2600.h"
#include "systematari520st.h"
#include "source/LeLib/util/cinifile.h"

class FactorySystem
{
public:
    FactorySystem();

    static AbstractSystem* Create(AbstractSystem::System type,QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) {
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
        if (type==AbstractSystem::PLUS4)
            return new SystemPlus4(settings, proj);
        if (type==AbstractSystem::OK64)
            return new SystemOK64(settings, proj);
        if (type==AbstractSystem::X16)
            return new SystemX16(settings, proj);
        if (type==AbstractSystem::X86)
            return new SystemX86(settings, proj);
        if (type==AbstractSystem::GAMEBOY)
            return new SystemGameboy(settings, proj);
        if (type==AbstractSystem::SPECTRUM)
            return new SystemSpectrum(settings, proj);
        if (type==AbstractSystem::TIKI100)
            return new SystemTiki100(settings, proj);
        if (type==AbstractSystem::ATARI2600)
            return new SystemAtari2600(settings, proj);
        if (type==AbstractSystem::ATARI520ST)
            return new SystemAtari520ST(settings, proj);


        return s;
    }


};

#endif // FACTORYSYSTEM_H
