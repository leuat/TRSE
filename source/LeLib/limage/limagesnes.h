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

    void SaveBin(QFile &file) override;
    void LoadBin(QFile &file) override;


    void ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center) override;

    void setPixel(int x, int y, unsigned int color) override;

    void ExportBin(QFile &file) override;


    void SetPalette(int pal) override;
    virtual bool isNes() override {return true;}
    QStringList getPaletteNames() override;

};

#endif // LIMAGESNES_H
