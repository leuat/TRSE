#ifndef SYSTEMAMIGA_H
#define SYSTEMAMIGA_H

#include "systemm6800.h"

class SystemAmiga : public SystemM68000
{
public:
    SystemAmiga(CIniFile* settings, CIniFile* proj) : SystemM68000(settings, proj) {
        m_processor = M68000;
        m_system = AMIGA;
    }
};

#endif // SYSTEMAMIGA_H
