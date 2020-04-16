#ifndef LIMAGEGAMBOY_H
#define LIMAGEGAMBOY_H

#include "limagenes.h"


class LImageGamboy : public LImageNES
{
public:
    LImageGamboy(LColorList::Type t);

    void ExportBin(QFile &file);

    void SpritePacker(LImage* in, QByteArray& sprData, int x, int y, int w, int h, int c) override;
    int SearchForIdenticalPixelChar(PixelChar o1, PixelChar o2, int compare);


    void ImportBin(QFile &file) override;

};

#endif // LIMAGEGAMBOY_H
