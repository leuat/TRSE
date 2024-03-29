#ifndef LIMAGELEVELSNES_H
#define LIMAGELEVELSNES_H

#include "imageleveleditor.h"

class LImageLevelSNES : public ImageLevelEditor
{
public:
    LImage* m_charset = nullptr;
    LImageLevelSNES(LColorList::Type t);
    unsigned int getPixel(int x, int y) override;
    void LoadCharset(QString file, int skipBttes) override;
    void InitPens() override;
    void ToQImage(LColorList& lst, QImage& img, double zoom, QPointF center) override;
    void CopyFrom(LImage *mc) override;
    LImage* getCharset() override;
    void ReInitialize() override;

    void ExportBin(QFile &file) override;

};

#endif // LIMAGELEVELSNES_H
