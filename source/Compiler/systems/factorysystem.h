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
#include "systemx16.h"
#include "systemgameboy.h"
#include "systemspectrum.h"
#include "systemtiki100.h"
#include "systematari2600.h"
#include "systematari520st.h"
#include "systemamstradcpc.h"
#include "systembbcm.h"
#include "systemcoleco.h"
#include "systematari800.h"
#include "systemmsx.h"
#include "systemappleii.h"
#include "systemm1arm.h"
#include "systemoric.h"
#include "systemsnes.h"
#include "systemmega65.h"
#include "systemcustom.h"
#include "systemvz200.h"
#include "systemacorn.h"
#include "systemjdh8.h"
#include "systempokemonmini.h"
#include "source/LeLib/util/cinifile.h"

class FactorySystem
{
public:
    FactorySystem();

    static AbstractSystem* Create(AbstractSystem::System type,QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);


};

#endif // FACTORYSYSTEM_H
