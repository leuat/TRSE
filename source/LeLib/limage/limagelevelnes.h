#ifndef LIMAGELEVELNES_H
#define LIMAGELEVELNES_H

#include "imageleveleditor.h"


class LImageLevelNES : public ImageLevelEditor
{
public:
    LImageLevelNES(LColorList::Type t);

    void SetColor(uchar col, uchar idx) override;



    unsigned int getPixel(int x, int y) override;
 //   void setPixel(int x, int y, unsigned int color);


    void ExportBin(QFile &file);


    void setPixel(int x, int y, unsigned int color) override;

    QString getMetaInfo();

    void Initialize() override;

    void LoadBin(QFile &file) override;

    void SetBank(int bank) override;

    void LoadCharset(QString file, int skipBttes) override;

};

#endif // LIMAGELEVELNES_H
