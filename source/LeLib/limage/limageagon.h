#ifndef LIMAGEAGON_H
#define LIMAGEAGON_H

#include "limagegeneric.h"

class LImageAgon : public LImageGeneric
{
public:

    LImageAgon(LColorList::Type t);
    void LoadBin(QFile& file) override;

    void SaveBin(QFile& file) override;

    QString getMetaInfo() override;
    void ExportBin(QFile &file) override;

private:
};

#endif // LIMAGEGENERIC_H
