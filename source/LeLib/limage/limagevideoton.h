#ifndef LIMAGEVIDEOTON_H
#define LIMAGEVIDEOTON_H

#include "limageqimage.h"
#include <QMatrix4x4>
#include "pixelchar.h"

class LImageVideoton : public LImageQImage
{
public:
    LImageVideoton(LColorList::Type t);


    static QVector<int> table160;



    static uchar CrazySwap(uchar c);


    QVector<int> m_data;
    unsigned char *temp_data = nullptr;

    virtual void SetMode();
    int m_colors;
//    virtual void InitPens() override;

//    void setPixel(int x, int y, unsigned int color) override;

    void ToQImage(LColorList& lst, QImage& img, double zoom, QPointF center) override;

    void InitPens() override;

    void ReInitialize()
    {
        m_width = getMetaParameter("screen_width")->value;
        m_height = getMetaParameter("screen_height")->value;
        m_colors = getMetaParameter("colors")->value;
        m_qImage = nullptr;
        SetMode();
    }
    //    void OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, QPoint size, float gamma=1.0) override;

    void ExportBin(QFile& ofile) override;
    void LoadBin(QFile& file) override;
    void SaveBin(QFile& file) override;


};

#endif // LIMAGEVIDEOTON_H
