#ifndef LIMAGEAMSTRADCPC_H
#define LIMAGEAMSTRADCPC_H

#include "limageqimage.h"

class LImageAmstradCPC : public LImageQImage
{
public:
    LImageAmstradCPC(LColorList::Type t);

    QVector<int> m_data;

    void SetMode(int mode);
    int m_colors;
    int m_bpp;

    virtual void InitPens() override;

    void setPixel(int x, int y, unsigned int color) override;

//    unsigned int getPixel(int x, int y)  override;

    void ToQImage(LColorList& lst, QImage& img, float zoom, QPointF center) override;

    void fromQImage(QImage *img, LColorList &lst);


};

#endif // LIMAGEAMSTRADCPC_H
