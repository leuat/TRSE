#ifndef METHODS68000ATARI_H
#define METHODS68000ATARI_H

#include "methods68000.h"

class Methods68000Atari : public Methods68000
{
public:
    Methods68000Atari();
//    bool Command(QString name) override;
    void Assemble(Assembler *as, AbstractCodeGen *dispatcher);

};

#endif // METHODS68000ATARI_H
