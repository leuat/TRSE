#ifndef METHODS6502OK64_H
#define METHODS6502OK64_H

#include "methods6502.h"

class Methods6502OK64 : public Methods6502
{
public:
    Methods6502OK64();

    void Assemble(Assembler* as, AbstractCodeGen* dispenser) override;


    void WaitForVerticalBlank(Assembler* as);

    void CallOKVC(Assembler *as, int noParams, uchar val);

    void toColor(Assembler *as);


};
#endif // METHODS6502OK64_H
