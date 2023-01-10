#ifndef LIMAGETIM_H
#define LIMAGETIM_H

#include "limageqimage.h"

class LImageTIM : public LImageQImage
{
public:
    LImageTIM(LColorList::Type t);


    void ExportBin(QFile &file) override;

//    QStringList SpriteCompiler(QString name, QString currentDir, QString src, QString dst, int x, int y, int w, int h) override;


};

#endif // LIMAGEVZ200_H
