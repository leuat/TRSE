#ifndef METHODS6502C64_H
#define METHODS6502C64_H

#include "methods6502.h"

class Methods6502C64 : public Methods6502
{
public:
    Methods6502C64();

    void Assemble(Assembler* as, AbstractCodeGen* dispenser) override;

    void WaitForVerticalBlank(Assembler* as);
    void Jammer(Assembler *as);

    void FLD(Assembler *as);

};

#endif // METHODS6502C64_H
