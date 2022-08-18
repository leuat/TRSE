#ifndef LIMAGECGA160X100_H
#define LIMAGECGA160X100_H

#include "limageqimage.h"

class LImageCGA160x100 : public LImageQImage
{
public:
    LImageCGA160x100(LColorList::Type t);

    void ExportBin(QFile &file) override;

};

#endif // LIMAGECGA160X100_H
