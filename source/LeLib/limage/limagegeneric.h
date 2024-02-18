#ifndef LIMAGEGENERIC_H
#define LIMAGEGENERIC_H

#include "limageqimage.h"

class LImageGeneric : public LImageQImage
{
public:

    LImageGeneric(LColorList::Type t);
    virtual void LoadBin(QFile& file) override;

    virtual void SaveBin(QFile& file) override;

    virtual QString getMetaInfo() override;

    void ExportBin(QFile &file) override;

private:
};

#endif // LIMAGEGENERIC_H
