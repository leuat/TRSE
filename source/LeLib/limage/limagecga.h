#ifndef LIMAGECGA_H
#define LIMAGECGA_H

#include "source/LeLib/limage/limageqimage.h"
#include "source/LeLib/util/util.h"
class LImageCGA : public LImageQImage
{
public:

    LImageCGA(LColorList::Type t);

    void ExportBin(QFile &file) override;

    QStringList SpriteCompiler(QString name, QString src, QString dst, int x, int y, int w, int h) override;

    void toCGA(QByteArray& even, QByteArray& odd,QByteArray& evenMask, QByteArray& oddMask, int x1, int y1, int w, int h);


};

#endif // LIMAGECGA_H
