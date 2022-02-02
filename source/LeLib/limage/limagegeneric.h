#ifndef LIMAGEGENERIC_H
#define LIMAGEGENERIC_H

#include "limageqimage.h"

class LImageGeneric : public LImageQImage
{
public:

    LImageGeneric(LColorList::Type t);
    void LoadBin(QFile& file) override;

    void SaveBin(QFile& file) override;

    QString getMetaInfo() override;

private:
};

#endif // LIMAGEGENERIC_H
