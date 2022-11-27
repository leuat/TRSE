#ifndef LIMAGEAMSTRADSPRITES_H
#define LIMAGEAMSTRADSPRITES_H
#include <QKeyEvent>
#include "source/LeLib/limage/multicolorimage.h"

#include "source/LeLib/limage/charsetimage.h"
#include "source/LeLib/util/util.h"
#include "source/LeLib/limage/limagecontainer.h"
#include "source/LeLib/limage/limagegenericsprites.h"
#include "source/LeLib/limage/limageamstradcpc.h"


class LAmstradSprite : public LGenericSprite {
    QByteArray ToQByteArray(int mask) override {
        QByteArray ba;
        return ba;
    }

};

class LImageAmstradSprites : public LImageGenericSprites
{
public:
    LImageAmstradSprites(LColorList::Type t);
//    LImageSprites(LColorList::Type t);

//    LImageSprites2() {}


    void ExportBin(QFile &ofile);
    QByteArray ExportFrame(int i);

    void ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center) override;


};

#endif // LIMAGEAMSTRADSPRITES_H
