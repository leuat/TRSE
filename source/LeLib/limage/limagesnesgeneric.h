#ifndef LIMAGESNESGENERIC_H
#define LIMAGESNESGENERIC_H

#include "limagesnes.h"

class LImageSnesGeneric : public LImageSNES
{
public:
    LImageSnesGeneric(LColorList::Type t);
    void SaveBin(QFile &file) override;
    void LoadBin(QFile &file) override;

    void ReInitialize() override;


};

#endif // LIMAGESNESGENERIC_H
