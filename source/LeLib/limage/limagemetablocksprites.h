#ifndef LIMAGEMETABLOCKSPRITES_H
#define LIMAGEMETABLOCKSPRITES_H

#include "source/LeLib/limage/limagemetachunk.h"


class LImageMetaBlockSprites : public LImageMetaChunk
{
public:
    LImageMetaBlockSprites(LColorList::Type t);

    void SaveBin(QFile &file) override;

    void LoadBin(QFile &file) override;


    void setPixel(int x, int y, unsigned int color) override;




    unsigned int getPixel(int x, int y) override;

    void ConstrainColours(QVector<int>& cols) override {

    }

    void ExportBin(QFile &file) override;

    virtual void FlipHorizontal() override;
    virtual void FlipVertical() override;


};

#endif // LIMAGEMETABLOCKSPRITES_H
