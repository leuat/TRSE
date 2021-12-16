#ifndef LIMAGECUSTOMC64MULTICOLOR_H
#define LIMAGECUSTOMC64MULTICOLOR_H

#include "multicolorimage.h"


class LImageCustomC64 : public MultiColorImage
{
public:
    LImageCustomC64(LColorList::Type t);

    void SaveBin(QFile& file) override;

    void LoadBin(QFile& file) override;

    QString getMetaInfo() override;

    void ReInitialize() override;

    void ExportBin(QFile& ofile) override;


};

#endif // LImageCustomC64_H
