#ifndef LIMAGESNES_H
#define LIMAGESNES_H

#include "source/LeLib/limage/limageamiga4.h"
#include <QKeyEvent>

class LImageSNES : public LImageAmiga
{
public:
    LImageSNES();

    bool m_firstIgnoreDone = false;
    LImageSNES(LColorList::Type t);

    void InitPens() override;

    void Initialize(int width, int height) override;

    void SaveBin(QFile &file) override;
    void LoadBin(QFile &file) override;
    QStringList getBankNames() override;

    virtual void SetBank(int bnk) override;

//    unsigned int getPixelPalette(int x, int y, int pal) override;

    void ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center) override;

    void setPixel(int x, int y, unsigned int color) override;

    void ExportBin(QFile &file) override;
    LImage* getCharset() override { return this; }

    void ReInitialize() override;
    void SetPalette(int pal) override;
    virtual bool isNes() override;
    virtual bool isSnes() override;
    QStringList getPaletteNames() override;
    QString GetCurrentDataString() override;

};

#endif // LIMAGESNES_H
