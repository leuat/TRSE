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
    bool m_ignoreConstraints = false;
    void OrdererdDither(QImage& img, LColorList& colors, QVector3D strength, QPoint size,float gamma) override;

    void LoadBin(QFile& file) override;
    void ExportBin(QFile& ofile) override;
    void getColors(int x, int y, int& c1, int& c2, int &b);

};

#endif // LIMAGESPECTRUM_H
