#ifndef METHODS6800AMIGA_H
#define METHODS6800AMIGA_H


#include "methods68000.h"

class Methods68000Amiga : public Methods68000
{
public:
    Methods68000Amiga();
    void Assemble(Assembler *as, AbstractASTDispatcher *dispatcher);

};

#endif // METHODS6800AMIGA_H
