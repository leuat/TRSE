#ifndef LIMAGENES_H
#define LIMAGENES_H

#include "source/LeLib/limage/multicolorimage.h"
#include "source/LeLib/limage/charsetimage.h"
#include <QKeyEvent>

class LImageNES : public CharsetImage
{
public:
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

    void SaveBin(QFile& file) override;
    void ConstrainColours(QVector<int>& cols) override {
        PerformConstrainColours(cols);

    }
    virtual QStringList getBankNames() override;

    void CopySingleChar(LImage* src, int srcChar, int dstChar) override;


    virtual bool isNes() override {return true;}


    bool KeyPress(QKeyEvent *e) override;
    void LoadBin(QFile& file) override;

    QPixmap ToQPixMap(int chr) override;
    void SetPalette(int pal) override;

    bool getXY(QPoint& xy,QPoint& p1, QPoint& p2);

    virtual void InitPens() override;

   PixelChar &getPixelChar(int x, int y) override;



    virtual QString getMetaInfo() override;


   void CompressAndSave(QByteArray& chardata, QVector<int>& screen, int x0,int x1, int y0, int y1, int& noChars, double compression, int maxChars, int type, bool AddChars) override;



};

#endif // LIMAGENES_H
