#ifndef LIMAGESPECTRUM_H
#define LIMAGESPECTRUM_H

#include "source/LeLib/limage/limageqimage.h"

class LImageSpectrum : public LImageQImage
{
public:
    LImageSpectrum(LColorList::Type t);
    void setPixel(int x, int y, unsigned int color) override;
    void InitPens() override;
    void MaintainSpectrumAttribute(int x,int y);

};

#endif // LIMAGESPECTRUM_H
