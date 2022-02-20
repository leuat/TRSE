#ifndef LIMAGEVZ200_H
#define LIMAGEVZ200_H

#include "limageqimage.h"

class LImageVZ200 : public LImageQImage
{
public:
    LImageVZ200(LColorList::Type t);


    void ExportBin(QFile &file) override;

    QStringList SpriteCompiler(QString name, QString currentDir, QString src, QString dst, int x, int y, int w, int h);


};

#endif // LIMAGEVZ200_H
