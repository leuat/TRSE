#ifndef LIMAGECGA_H
#define LIMAGECGA_H

#include "source/LeLib/limage/limageqimage.h"

class LImageCGA : public LImageQImage
{
public:

    LImageCGA(LColorList::Type t);

    void ExportBin(QFile &file) override;

};

#endif // LIMAGECGA_H
