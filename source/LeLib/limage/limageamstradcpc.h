#ifndef LIMAGEAMSTRADCPC_H
#define LIMAGEAMSTRADCPC_H

#include "limageqimage.h"
#include <QMatrix4x4>
#include "pixelchar.h"

class LImageAmstradCPC : public LImageQImage
{
public:
    LImageAmstradCPC(LColorList::Type t);


    QVector<int> table160 =  {0b00000000, 0b00000001,0b00000100,0b00000101,
                              0b00010000, 0b00010001,0b00010100,0b00010101,
                              0b01000000, 0b01000001,0b01000100,0b01000101,
                              0b01010000, 0b01010001,0b01010100,0b01010101};




    static uchar AmstradCrazySwap(uchar c);

    QVector<int> m_data;

    void SetMode(int mode);
    int m_colors;
    int m_bpp;

    virtual void InitPens() override;

    void setPixel(int x, int y, unsigned int color) override;

    void ToQImage(LColorList& lst, QImage& img, float zoom, QPointF center) override;

    void OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, int size, float gamma=1.0) override;

    void ExportBin(QFile& ofile) override;
    void LoadBin(QFile& file) override;


};

#endif // LIMAGEAMSTRADCPC_H
