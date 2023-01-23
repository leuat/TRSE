#ifndef LIMAGEGAMBOY_H
#define LIMAGEGAMBOY_H

#include "limagenes.h"


class LImageGamboy : public LImageNES
{
public:
    LImageGamboy(LColorList::Type t);
    bool m_isInitalised = false;
    void ExportBin(QFile &file) override;

    void SpritePacker(LImage* in, QByteArray& rawDataOut, QByteArray& sprData, int x, int y, int w, int h, int c,int& noChars) override;
    int SearchForIdenticalPixelChar(PixelChar o1, PixelChar o2, int compare);


    void ImportBin(QFile &file) override;
    void Initialize(int width, int height) override;

    void InitPens() override;

};

#endif // LIMAGEGAMBOY_H
