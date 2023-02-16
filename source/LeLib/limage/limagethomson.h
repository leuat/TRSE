#ifndef LIMAGETHOMSON_H
#define LIMAGETHOMSOM_H

#include "limageqimage.h"
#include <QMatrix4x4>
#include "pixelchar.h"

class LImageThomson : public LImageQImage
{
public:
    LImageThomson(LColorList::Type t);


    int lcols [16]  {
        0b0000,
        0b0001,
        0b0010,
        0b0011,
        0b0100,
        0b0101,
        0b0110  ,
        0b1111,
        0b1000,
        0b1001,
        0b1010,
        0b1011,
        0b1100,
        0b1101,
        0b1110,
        0b1111
    };




    QByteArray m_data;
    QByteArray m_cols;

    virtual void SetMode();
//    virtual void InitPens() override;

//    void setPixel(int x, int y, unsigned int color) override;

    void ToQImage(LColorList& lst, QImage& img, double zoom, QPointF center) override;

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;

    void ReInitialize() override
    {
        m_width = getMetaParameter("screen_width")->value;
        m_height = getMetaParameter("screen_height")->value;
        m_qImage = nullptr;
        SetMode();
    }
    //    void OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, QPoint size, float gamma=1.0) override;

    void ExportBin(QFile& ofile) override;
    void LoadBin(QFile& file) override;
    void SaveBin(QFile& file) override;
    void CopyFrom(LImage *img) override;


};

#endif // LIMAGECOCO3_H
