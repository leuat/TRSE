#ifndef LIMAGEPRIMO_H
#define LIMAGEPRIMO_H

#include "source/LeLib/limage/limageqimage.h"

class LImagePrimo : public LImageQImage
{
public:
    LImagePrimo(LColorList::Type t);
    void InitPens() override;
    bool m_ignoreConstraints = false;
//    void OrdererdDither(QImage& img, LColorList& colors, QVector3D strength, QPoint size,float gamma) override;

    void LoadBin(QFile& file) override;
    void SaveBin(QFile &file) override;
    void ExportBin(QFile& ofile) override;
    QString getMetaInfo() override ;

};

#endif // LIMAGESPECTRUM_H
