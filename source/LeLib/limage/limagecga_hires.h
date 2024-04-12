#ifndef LIMAGECGA_H_H
#define LIMAGECGA_H_H

#include "source/LeLib/limage/limagecga.h"
#include "source/LeLib/util/util.h"
class LImageCGAHires : public LImageCGA
{
public:

    LImageCGAHires(LColorList::Type t);

    void toCGA(QByteArray& even, QByteArray& odd, QByteArray& evenMask, QByteArray& oddMask,int x1, int y1, int w, int h) override;

};

#endif // LIMAGECGA_H
