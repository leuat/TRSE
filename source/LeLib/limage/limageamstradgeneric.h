#ifndef LIMAGEAMSTRADGENERIC_H
#define LIMAGEAMSTRADGENERIC_H

#include "limageamstradcpc.h"


class LImageAmstradGeneric : public LImageAmstradCPC
{
public:
    LImageAmstradGeneric(LColorList::Type t);
    void SetMode() override;
    void ReInitialize() override;

    void LoadBin(QFile& file) override;

    void SaveBin(QFile& file) override;

    void ExportBin(QFile &ofile) override;

};

#endif // LIMAGEAMSTRADGENERIC_H
