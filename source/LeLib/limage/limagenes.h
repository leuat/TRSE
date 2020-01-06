#ifndef LIMAGENES_H
#define LIMAGENES_H

#include "source/LeLib/limage/multicolorimage.h"
#include "source/LeLib/limage/charsetimage.h"

class LImageNES : public CharsetImage
{
public:
    uchar m_cols[4];
    LImageNES(LColorList::Type t);
    void ImportBin(QFile &file) override;
    void ExportBin(QFile &file) override;
    void setMultiColor(bool doSet) override;
    void ForceColor();
    bool m_double = true;
    unsigned int getPixel(int x, int y) override;
    void setPixel(int x, int y, unsigned int col) override;
    void SetColor(uchar col, uchar idx) override;
    void CopyFrom(LImage* img) override;
    void setForeground(unsigned int col) override;

    void SaveBin(QFile& file) override;
    void ConstrainColours(QVector<int>& cols) override {
        PerformConstrainColours(cols);

    }

    void LoadBin(QFile& file) override;
    void ApplyColor() override {

    }
    QPixmap ToQPixMap(int chr) override;
    void SetPalette(int pal) override;

    bool getXY(QPoint& xy,QPoint& p1, QPoint& p2);


    virtual QString getMetaInfo() override;


};

#endif // LIMAGENES_H
