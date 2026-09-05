#ifndef LIMAGEORIC_H
#define LIMAGEORIC_H

#include "limageqimage.h"
#include <QMatrix4x4>

class LImageOric : public LImageQImage
{
public:
    LImageOric(LColorList::Type t);



    QVector<int> m_data;
    unsigned char *temp_data = nullptr;

    virtual void SetMode();
    int m_colors;
    int m_bpp;
    virtual void InitPens() override;

//    void setPixel(int x, int y, unsigned int color) override;

    void ToQImage(LColorList& lst, QImage& img, double zoom, QPointF center) override;


    void ExportBin(QFile& ofile) override;
    void LoadBin(QFile& file) override;


};

#endif // LIMAGEAMSTRADCPC_H
