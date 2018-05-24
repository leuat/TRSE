#ifndef STANDARDCOLORIMAGE_H
#define STANDARDCOLORIMAGE_H

#include "source/LeLib/limage/multicolorimage.h"

class StandardColorImage : public MultiColorImage
{
public:
    StandardColorImage(LColorList::Type t);
    void ExportAsm(QString filename) override;

};

#endif // STANDARDCOLORIMAGE_H
