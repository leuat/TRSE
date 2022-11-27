#ifndef LIMAGEAMSTRADCPC_H
#define LIMAGEAMSTRADCPC_H

#include "limageqimage.h"
#include <QMatrix4x4>
#include "pixelchar.h"

class LImageAmstradCPC : public LImageQImage
{
public:
    LImageAmstradCPC(LColorList::Type t);


    static QVector<int> table160;



    static uchar AmstradCrazySwap(uchar c);


    QVector<int> m_data;
    unsigned char *temp_data = nullptr;

    virtual void SetMode();
    int m_colors;
    int m_bpp;
    int m_mode;
    virtual void InitPens() override;

//    void setPixel(int x, int y, unsigned int color) override;

    void ToQImage(LColorList& lst, QImage& img, double zoom, QPointF center) override;

//    void OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, QPoint size, float gamma=1.0) override;

    void ExportBin(QFile& ofile) override;
    void LoadBin(QFile& file) override;


};

#endif // LIMAGEAMSTRADCPC_H
