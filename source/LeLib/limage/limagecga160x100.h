#ifndef LIMAGECGA160X100_H
#define LIMAGECGA160X100_H

#include "limageqimage.h"

class LImageCGA160x100 : public LImageQImage
{
public:
    LImageCGA160x100(LColorList::Type t);

    void ExportBin(QFile &file) override;


    void LoadBin(QFile &file) override;

    void SaveBin(QFile &file) override;

//    QStringList SpriteCompiler(QString name, QString currentDir, QString src, QString dst, int x, int y, int w, int h) override;
    void ReInitialize() override;


};

#endif // LIMAGECGA160X100_H
