#ifndef LIMAGEVIC20_H
#define LIMAGEVIC20_H

#include "source/LeLib/limage/multicolorimage.h"
#include "source/LeLib/limage/charsetimage.h"


class LImageVIC20 : public CharsetImage
{
public:
    LImageVIC20(LColorList::Type t);
    void setMultiColor(bool doSet) override;

    void SaveBin(QFile& file) override;
    void LoadBin(QFile& file) override;

    void setPixel(int x, int y, unsigned int color) override;
    void ToRaw(QByteArray &arr) override;


};

#endif // LIMAGEVIC20_H
